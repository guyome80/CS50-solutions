import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""

        # use of self declaration as a convention of instance variable
        self.positives = 0
        self.negatives = 0
        totalpos = 0
        totalneg = 0

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""

        # start from 0 for each Analyser variable
        self.positives = 0
        self.negatives = 0

        # precise self text value
        self.text = text

        # declare a tokenased word
        tokenizer = nltk.tokenize.TweetTokenizer()
        tokens = tokenizer.tokenize(text)

        # indicate the length of list tokens
        size = len(tokens)

        # all the word stuff to ckeck
        for word in tokens:

            # chaque mots est converti en mot sans majuscule
            word = str.lower(word)

            linespos = [line.rstrip('\n') for line in open('positive-words.txt')]
            linesneg = [line.rstrip('\n') for line in open('negative-words.txt')]

            # check for positive or negative or neutral words
            if word in linespos:
                self.positives += 1
            elif word in linesneg:
                self.negatives += 1
            else:
                continue

        # score calculculated and reurned
        score = self.positives - self.negatives

        return score