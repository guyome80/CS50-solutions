from flask import Flask, redirect, render_template, request, url_for

import helpers
import os
import sys
from analyzer import Analyzer

app = Flask(__name__)

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/search")
def search():

    # validate screen_name
    screen_name = request.args.get("screen_name", "")
    if not screen_name:
        return redirect(url_for("index"))

    # get screen_name's tweets
    tweets = helpers.get_user_timeline(screen_name)

    positives = os.path.join(sys.path[0], "positive-words.txt")
    negatives = os.path.join(sys.path[0], "negative-words.txt")

    if tweets is None:
        return redirect(url_for("index"))

    # initialize score for positive, negative and neutral tweets
    positive = 0.0
    negative = 0.0
    neutral = 0.0

    # instantiate analyzer
    analyzer = Analyzer(positives, negatives)

    # calculates the score
    for tweet in tweets:
        score = analyzer.analyze(tweet)

        #calculates total score positive, negative, neutral
        if score > 0.0:
            positive += 1
        elif score < 0.0:
            negative += 1
        else:
            neutral += 1

    # initial % for each 3 kinds of word
    # positive, negative, neutral = 0.0, 0.0, 100.0

    # generate chart
    chart = helpers.chart(positive, negative, neutral)

    # render results
    return render_template("search.html", chart=chart, screen_name=screen_name)
