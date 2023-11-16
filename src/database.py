import datetime
import libsql_client as sql

DB_URL = 'libsql://roomba-rush-db-conneroisu.turso.io'

# https://docs.turso.tech/libsql/client-access/python-sdk

# Gets the client for the database connection
def getClient() -> sql.Client:
    return sql.create_client_sync(
        url="libsql://roomba-rush-db-conneroisu.turso.io",
        auth_token='eyJhbGciOiJFZERTQSIsInR5cCI6IkpXVCJ9.eyJpYXQiOiIyMDIzLTEwLTI0VDIxOjE5OjA3LjY0NTU0MjEzWiIsImlkIjoiZWI3ZGVkN2QtNzJiMi0xMWVlLWFmNjYtYmFmMDU1NzM4YzU3In0.yFSS41a0GPAjAhNQLCQagd0ARpbf71Gb1zwjmSiqayKpm0D7p0mop3I_EgUF9sUFj-uFg39-H0eSapzppDN1BA'
    )


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
    client = getClient();
    with client:
        # Execute a query
        client.execute("insert into paths values (:date, :lat, :long)", {"date": date, "lat": lat, "long": long})


# Gets all the paths from the database
def getPaths() -> [Marker]:
    client = getClient()
    with client:
        # create marker array object
        markers = []
        # Execute a query
        result_set = client.execute("SELECT * FROM paths")
        # Work with query results
        for row in result_set.rows:
            markers.append(Marker(row[0], row[1], row[2]))

        return markers

# Gets the current date and time in the database format
def getDate() -> str:
    # Creates a string like  Wednesday, October 25, 2023 at 12:59 PM and 23.002 seconds
    return datetime.datetime.now().strftime("%A, %B %d, %Y at %I:%M %p and %S.%f seconds")


# if __name__ == '__main__':
#     res = getPaths()
#     # for r in res:
#     #     addPath(getDate(), r.lat, r.long)
#     for r in res:
#         print(r)
#
#     print(getPaths())
