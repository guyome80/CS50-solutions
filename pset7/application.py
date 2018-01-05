from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():

    # database from user to check his cash
    cash = db.execute("SELECT * FROM users WHERE id=:user_id", user_id=session["user_id"])[0]["cash"]

    # database check according to the exchange of the buyer and where the sum of his exchange is not null
    rows= db.execute("SELECT s.* FROM (SELECT symbol, SUM(shares) AS totalshares, price, totalvalue FROM exchange WHERE buyer_id=:user_id GROUP BY symbol) AS s WHERE s.totalshares > 0", user_id=session["user_id"])

    totalcapital = 0
    if len(rows) == 0:
        totalcapital = cash
        return render_template("index.html", cash=cash)
    # allows the addition to all the share capital value

    else:
        for symbolgroup in rows:
            symbol=lookup(symbolgroup["symbol"])
            symbolgroup["sharesname"]=symbol["name"]
            symbolgroup["price"]=symbol["price"]
            symbolgroup["shares"]=symbolgroup["totalshares"]
            symbolgroup["sharecapital"]= symbolgroup["price"] * symbolgroup["totalshares"]
            totalcapital = totalcapital + symbolgroup["sharecapital"]
            symbolgroup["sharecapital"]=usd(symbolgroup["sharecapital"])
            symbolgroup["price"]=usd(symbolgroup["price"])

        # add cash to total capital
        totalcapital = totalcapital + cash

        return render_template("index.html", cash=cash, rows=rows, symbol=symbol, totalcapital=usd(totalcapital))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # symbol to insert !
        if not request.form.get("symbol"):
            return apology("missing symbol")

        # shares to insert !
        elif not request.form.get("shares", type=int):
            return apology("invalid shares")

        try:
            val = request.form.get("shares")
            if int(val) < 0:
                return apology("invalide shares")
        except ValueError:
            return apology("invalide shares")

        # use the quote function in helpers to check the symbol
        if lookup(request.form.get("symbol"))==None:
            return apology("wrong symbol")

        # quote informations on the database (ex : quote['price'])
        quote = lookup(request.form.get("symbol"))

        # takes the user cash information
        cash = db.execute("SELECT * FROM users WHERE id=:user_id", user_id=session["user_id"])[0]["cash"]

        # check if user has enough money
        if cash < (int(val) * quote['price']):
            return apology("too expensive")

        # change the cash db value with the bought shares
        else:
            db.execute("INSERT INTO exchange (buyer_id,	symbol,	shares,	price) VALUES(:user_id, :symbol, :shares, :price)", user_id=session["user_id"], symbol=quote["symbol"].upper(), shares=request.form.get("shares"), price=quote['price'])

        db.execute("UPDATE users SET cash=:cash WHERE id=:user_id", cash= cash- (int(val) * quote['price']), user_id=session["user_id"])

        # message about shares bought
        flash('Bought !!')

        #user_id=session["user_id"])
        return redirect(url_for("index"))

    else:
        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""

    rows = db.execute("SELECT * FROM exchange WHERE buyer_id=:user_id", user_id=session["user_id"])
    # transform the value in usd value
    for row in rows:
        row["price"]=usd(row["price"])

    return render_template("history.html", rows=rows)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        if not request.form.get("symbol"):
            return apology("missing symbol")

        # use the quote function in helpers to check the symbol
        if lookup(request.form.get("symbol"))==None:
            return apology("wrong symbol")

        quote = lookup(request.form.get("symbol"))
        quote['price'] = usd(quote['price'])

        return render_template("quote.html", quote=quote)

    else:
        return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide", "username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide", "password")

        # ensure the second password was submitted
        elif not request.form.get("password confirmation"):
            return apology("passwords", "don't match")

        # ensure first and second password are the same
        elif request.form.get("password") != request.form.get("password confirmation"):
            return apology("passwords", "don't match")

        # convert password in hashed
        hash = pwd_context.hash("password")
        ok = pwd_context.verify("passeword", hash)

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # check if username already taken
        if len(rows) == 1:
            return apology("username", "exists...")

        # insert username and hashed password in the database if
        else:
            db.execute("INSERT INTO users (username, hash) VALUES(:username, :password)", username=request.form.get("username"), password=pwd_context.hash(request.form.get("password")))

        # loads the user informations
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""# if user reached route via POST (as by submitting a form via POST)

    if request.method == "POST":

        # symbol to insert !
        if not request.form.get("symbol"):
            return apology("missing symbol")

        # shares to insert !
        elif not request.form.get("shares", type=int):
            return apology("invalid/missing", "shares")

        try:
            val = request.form.get("shares")
            if int(val) < 0:  # if not a positive int print message and ask for input again
                return apology("invalide shares")
        except ValueError:
            return apology("invalide shares")

        # check if symbol is in the yahoo finance database
        sell=lookup(request.form.get("symbol"))
        if sell==None:
            return apology("wrong symbol")

        # loads database exchange
        rows = db.execute("SELECT symbol, SUM(shares), price FROM exchange WHERE buyer_id=:user_id AND symbol=:symbol GROUP BY symbol=:symbol", user_id=session["user_id"], symbol=sell["symbol"])

        # check if symbols are available to sell
        if len(rows) != 1:
            return apology("not available","symbol")

        # check if enough of this symbol to sell
        if rows[0]['SUM(shares)'] < int(request.form.get("shares")):
            return apology("too numerous","shares")

        # loads user database to get the cash
        cash = db.execute("SELECT * FROM users WHERE id=:user_id", user_id=session["user_id"])[0]["cash"]

        # integer the sell informations into the exchange database
        db.execute("INSERT INTO exchange (buyer_id,	symbol,	shares,	price) VALUES(:user_id, :symbol, :shares, :price)", user_id=session["user_id"], symbol=sell["symbol"].upper(), shares=0-int(val), price=sell['price'])

        # change the cash available money for the user
        db.execute("UPDATE users SET cash=:cash WHERE id=:user_id", cash= cash + (int(val) * sell['price']), user_id=session["user_id"])

        # message about shares sold
        flash('Sold !!')

        #user_id=session["user_id"])
        return redirect(url_for("index")) #, buy=buy)

    else:
        return render_template("sell.html")

@app.route("/newpassword", methods=["GET", "POST"])
@login_required
def newpassword():
    """Allows to change password."""# if user reached route via POST (as by submitting a form via POST)

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure password was submitted
        if not request.form.get("newpassword"):
            return apology("must provide", "password")

        # ensure the second password was submitted
        elif not request.form.get("newpassword confirmation"):
            return apology("passwords", "don't match")

        # ensure first and second password are the same
        elif request.form.get("newpassword") != request.form.get("newpassword confirmation"):
            return apology("passwords", "don't match")

        # convert password in hashed
        hash = pwd_context.hash("newpassword")
        ok = pwd_context.verify("newpasseword", hash)

        db.execute("UPDATE users SET hash=:password WHERE id=:user_id", password=pwd_context.hash(request.form.get("newpassword")),user_id=session["user_id"])

        # message about shares sold
        flash('Password changed !!')

        #user_id=session["user_id"])
        return redirect(url_for("index"))
    else:
        return render_template("newpassword.html")