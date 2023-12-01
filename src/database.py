import datetime
import xata

load_dotenv.load_dotenv()


DB_URL = 'https://connero-s-workspace-ep1498.us-east-1.xata.sh/db/roomba-dash'
from xata.client import XataClient
import xata
from dotenv import load_dotenv

DB_URL = "libsql://roomba-rush-db-conneroisu.turso.io"

load_dotenv()

# Gets the client for the database connection
def getClient() -> XataClient:
    return xata.Client(DB_URL)
    

def getClient() -> XataClient:
    return XataClient(db_url="REDACTED_DB_URL")

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
    return client.records().insert("paths", record, "main")


# Gets all the paths from the database
def getPaths():
    client = getClient()
    records = client.records()
    results = records.get("paths", "all")
    return results


# Gets the current date and time in the database format


def getDate() -> str:
    # Creates a string like  Wednesday, October 25, 2023 at 12:59 PM and 23.002 seconds
    return datetime.datetime.now().strftime(
        "%A, %B %d, %Y at %I:%M %p and %S.%f seconds"
    )


# if __name__ == '__main__':
#     res = getPaths()
#     # for r in res:
#     #     addPath(getDate(), r.lat, r.long)
#     for r in res:
#         print(r)
#
#     print(getPaths())
