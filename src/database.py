import datetime
import os
from xata.client import XataClient
import xata
from dotenv import load_dotenv


load_dotenv()


def getClient() -> XataClient:
    DB_URL = os.getenv("DB_URL")
    API_KEY = os.getenv("API_KEY")
    if DB_URL is None:
        raise ValueError("DB_URL is not defined")
    if API_KEY is None:
        raise ValueError("API_KEY is not defined")
    return XataClient(db_url=DB_URL, api_key=API_KEY)


# marker has a date, lat, long
class Marker:
    def __init__(self, date, lat, long):
        self.date = date
        self.lat = lat
        self.long = long

    def __str__(self):
        return f"{self.date} {self.lat} {self.long}"


# Adds a path marker to the database which shows the path the robot has taken
def addPathMarker(date, lat, long) -> bool:
    client = getClient()
    record = {"date": date, "lat": lat, "long": long}
    client.records().insert("paths", record, "main")
    return True


# Gets all the paths from the database
def getPaths():
    client = getClient()
    records = client.records()
    results = records.get("paths", "all")
    return results


def getDestination():
    client = getClient()
    records = client.records()
    results = records.get("order", "all")
    return results

# Gets the current date and time in the database format


def getDate() -> str:
    # Creates a string like  Wednesday, October 25, 2023 at 12:59 PM and 23.002
    return datetime.datetime.now().strftime(
        "%A, %B %d, %Y at %I:%M %p and %S.%f seconds"
    )


def getOrders():
    client = getClient()

    data = client.data().query("orders", {
        "columns": [
            "id",
            "goods",
            "total_price",
            "house",
            "client"
        ]
    })
    print(data)


def createObstacle(latitude: float, longitude: float):
    client = getClient()

    data = client.records().insert("obstacles", {
        "latitude": latitude,
        "longitude": longitude
    })
    return data


def getHouses():
    client = getClient()

    data = client.data().query("houses", {
        "columns": [
            "id",
            "latitude",
            "longitude",
            "photo",
            "name",
            "character"
        ]
    })
    return data
