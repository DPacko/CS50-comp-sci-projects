from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime
from helpers import apology, login_required, lookup, usd
from pytz import timezone

# Configure application
app = Flask(__name__)

# Ensure responses aren't cached


@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # create variables for each row in html table
    user = session["user_id"]
    # SELECT user's stock names bought from portfolio
    stkshar = db.execute(
        "SELECT symbol, SUM(shares) FROM portfolio WHERE foreign_key=:user GROUP BY symbol HAVING SUM(shares) > 0", user=user)

    shares = []
    stocks = []
    for sell in stkshar:
        stocks.append(sell['symbol'])
        shares.append(sell['SUM(shares)'])

    # create a list of the current prices of each stock
    prices = []
    for stock in stocks:
        price = lookup(stock)
        amount = price["price"]
        prices.append(amount)

    # Calculate total value of each holding. share * price
    totalval = []
    for q, a in zip(shares, prices):
        cash = q * a
        totalval.append(cash)

    total = []
    for value in totalval:
        price = usd(value)
        total.append(price)

    price = []
    for value in prices:
        amount = usd(value)
        price.append(amount)

    # user's current cash balance
    usercash = (db.execute("SELECT cash FROM users WHERE id = :id", id=user))[0]["cash"]

    # grand total. total value + cash
    result = 0
    for x in totalval:
        result += x

    grandtotal = usd(result + usercash)
    cash = usd(usercash)

    # pass in variables to html using render template
    return render_template("index.html", stocks=stocks, shares=shares, price=price, total=total, cash=cash, grandtotal=grandtotal)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        user = session["user_id"]

        stock = lookup(request.form.get("symbol"))
        # did user put in an input?
        if not stock:
            return apology("must provide stock")

        symbol = stock["symbol"]

        # check to see if the user's number input is valid (not blank, not character, not negative)
        num = request.form.get("shares")
        try:
            num = int(num)
            if num < 0:
                return apology("must provide positive number of shares")
        except ValueError:
            return apology("must provide shares as a number")

        # query database for cash in user's table
        cash = (db.execute("SELECT * FROM users WHERE id = :id", id=user))[0]["cash"]
        shares = int(request.form.get("shares"))

        name = stock["name"]
        price = stock["price"]
        stkprc = price * shares
        # date = datetime.now(timezone('America/New_York'))

        # check if user has enough funds
        if cash < stkprc:
            return apology("not enough funds to purchase")

        # add user's purchase into the portfolio database
        db.execute("INSERT INTO portfolio (foreign_key, symbol, name, shares, price, total, date) VALUES (:id, :symbol, :name, :shares, :price, :total, CURRENT_TIMESTAMP)",
                   id=user, symbol=symbol, name=name, shares=num, price=price, total=stkprc)

        # update user's funds to reflect the deduction of recently bought shares
        db.execute("UPDATE users SET cash = cash - :stkprc WHERE  id = :id", stkprc=stkprc, id=user)

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    user = session["user_id"]

    # access stocks bought/sold from portfolio table
    stocknames = db.execute("SELECT symbol FROM portfolio WHERE foreign_key=:user ORDER BY date DESC", user=user)

    # access the shares in portfolio
    shares = db.execute("SELECT shares FROM portfolio WHERE foreign_key=:user ORDER BY date DESC", user=user)

    # access the current price which stock was bought/sold
    prices = db.execute("SELECT price FROM portfolio WHERE foreign_key=:user ORDER BY date DESC", user=user)

    currentprices = []
    for price in prices:
        for key, value in price.items():
            currentprices.append(usd(value))

    times = db.execute("SELECT date FROM portfolio WHERE foreign_key=:user ORDER BY date DESC", user=user)

    # pass in variables to html using render template
    return render_template("history.html", stocks=stocknames, shares=shares, currentprices=currentprices, times=times)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
# User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        quote = lookup(request.form.get("symbol"))
        #  ensure stock is valid
        if not quote:
            return apology("symbol")
        else:
            price = usd(quote["price"])
            # pass python value to html
            return render_template("quoted.html", name=quote, price=price)

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted correctly
        if not request.form.get("username"):
            return apology("missing username!")

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("missing password!")

        if request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords do not match")

        # Query database if username already exists
        result = db.execute("SELECT username FROM users WHERE username = :username", username=request.form.get("username"))

        if result:
            return apology("username already exists")

        # store hash of user's password for security
        hash = generate_password_hash(request.form.get("password"))

        id = db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)",
                        username=request.form.get("username"), hash=hash)

        # store their id number in session
        session["user_id"] = id

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":

        sym = request.form.get("symbol")
        user = session["user_id"]

        # check to see if the user's number input is valid (not blank, not character, not negative)
        if request.form.get("shares").isalpha():
            return apology("must provide shares as a number")

        sharerequest = int(request.form.get("shares"))

        # Query database if user has enough shares
        shares = db.execute("SELECT SUM(shares) FROM portfolio WHERE foreign_key=:user AND symbol=:sym", user=user, sym=sym)
        shareamount = []
        for share in shares:
            for key, value in share.items():
                shareamount.append(value)

        # check if proper amount of shares, if so then complete transaction
        for share in shareamount:
            if sharerequest <= 0:
                return apology("must provide positive number of shares")
            elif sharerequest > share:
                return apology("not enough shares")

            # lookup price of stock
            stock = lookup(sym)
            price = stock["price"]
            # deduct shares from overall user share quantity
            minusshares = -(sharerequest)
            stkprc = price * sharerequest

            # add user's sold stocks into the portfolio database
            db.execute("INSERT INTO portfolio (foreign_key, symbol, name, shares, price, total, date) VALUES (:id, :sym, :name, :shares, :price, :total, CURRENT_TIMESTAMP)",
                       id=user, sym=sym, name=sym, shares=minusshares, price=price, total=stkprc)

            # update user's funds to reflect the deduction of recently sold shares
            db.execute("UPDATE users SET cash = cash + :stkprc WHERE id = :id", stkprc=stkprc, id=user)

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        # did user put in an input?
        user = session["user_id"]

        stkshar = db.execute(
            "SELECT symbol, SUM(shares) FROM portfolio WHERE foreign_key=:user GROUP BY symbol HAVING SUM(shares) > 0", user=user)

        usersymbols = []
        for stock in stkshar:
            usersymbols.append(stock['symbol'])

        return render_template("sell.html", usersymbols=usersymbols)


@app.route("/settings", methods=["GET", "POST"])
@login_required
def settings():
    """Change settings"""
    return render_template("settings.html")


@app.route("/changepassword", methods=["GET", "POST"])
@login_required
def changepassword():
    if request.method == "POST":

        # Ensure username was submitted correctly
        if not request.form.get("oldpassword"):
            return apology("missing password!")

        # Ensure password was submitted
        elif not request.form.get("password1"):
            return apology("missing password!")

        if request.form.get("password1") != request.form.get("password2"):
            return apology("passwords do not match")

        # store hash of user's password for security
        hash = generate_password_hash(request.form.get("password1"))

        id = session["user_id"]

        newhash = db.execute("UPDATE users SET hash=:hash WHERE id=:id",
                             hash=hash, id=id)

        return render_template("pwdchangcom.html")
    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("changpwd.html")


def errorhandler(e):
    """Handle error"""
    return apology(e.name, e.code)


# listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
