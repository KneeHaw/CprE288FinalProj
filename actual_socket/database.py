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


def getOrder():
    """Returns a list of orders with the following columns: id, goods,
    total_price, house, client. The house and client columns are
    foreign keys to the houses and clients table. The orders are the
    orders that are in the database to be delivered."""
    # Get the xata client
    client = getClient()

    data = client.data().query(
        "orders",
        {
            "columns": [
                "id",
                "goods",
                "total_price",
                "house",
                "client",
                "house.character",
            ]
        },
    )
    # Reduce to one order
    print(data)
    return data


def getLatestOrder():
    """Returns the latest order in the database"""
    # Get the xata client
    client = getClient()

    # Quert Database for orders
    data = client.data().query(
        "orders",
        {
            "columns": [
                "house.character",
            ],
        },
    )

    # Reduce to one order
    data = data.get("records")
    # now data is a list
    data = data[0]
    data = data.pop("house")
    data = data.pop("character")
    # Return the data
    return data


def createObstacle(latitude: float, longitude: float):
    """Creates an obstacle with the given latitude and longitude
    in the database. Returns the data of the obstacle. Is
    Shown on the map as a obstacle"""

    # Get the xata client
    client = getClient()

    # Insert the obstacle
    data = client.records().insert(
        "obstacles", {"latitude": latitude, "longitude": longitude}
    )

    # Return the data
    return data


def getHouses():
    """Returns a list of houses with the following columns:
    id, latitude, longitude, photo, name, character"""

    # Get the xata client
    client = getClient()

    # Quert Database for houses
    data = client.data().query(
        "houses",
        {"columns": ["id", "latitude", "longitude", "photo", "name", "character"]},
    )
    return data


def updateOrderStatus(order_id: str, status: str):
    """A Status can be: "pending", "in_progress", "completed" or "cancelled"""
    # Get the xata client
    client = getClient()

    # Update the order status
    data = client.records().update("order", order_id, {"status": status})

    # Return the data
    return data


def updateObstacle(obstacle_id: str, latitude: float, longitude: float):
    """Updates the obstacle with the given id with the given latitude and longitude"""
    # Get the xata client
    client = getClient()

    # Update the obstacle
    data = client.records().update(
        "obstacles", obstacle_id, {"latitude": latitude, "longitude": longitude}
    )

    # Return the data
    return data


def deleteObstacle(obstacle_id: str):
    """Deletes the obstacle with the given id"""
    # Get the xata client
    client = getClient()

    # Delete the obstacle
    data = client.records().delete("obstacles", obstacle_id)

    # Return the data
    return data


def getObstacles():
    """Returns a list of obstacles with the following columns:
    id, latitude, longitude"""

    # Get the xata client
    client = getClient()

    # Quert Database for obstacles
    data = client.data().query(
        "obstacles", {"columns": ["id", "latitude", "longitude"]}
    )
    return data


def getObstacle(obstacle_id: str):
    """Returns the obstacle with the given id"""
    # Get the xata client
    client = getClient()

    # Quert Database for obstacle
    data = client.data().query(
        "obstacles",
        {"columns": ["id", "latitude", "longitude"], "filter": {"id": obstacle_id}},
    )
    return data
