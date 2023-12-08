// author: Conner Ohnesorge (2023) | conneroh.com
import { obstacleIcon } from "./map-icons";

import * as L from "leaflet";

// The records of the obstacles.
let records = [];
// The markers of the obstacles.
let markers = [];

/**
 * The function adds an obstacle marker to the map.
 * @param record The record containing the latitude and longitude of the obstacle.
 * @param map The map to add the marker to.
 */
function AddObstacle(record, map: L.Map) {
  // Add a marker to the map
  let marker = L.marker([record.latitude, record.longitude], {
    icon: obstacleIcon,
  }).addTo(map);
  markers.push(marker);
  // Add a popup to the marker
  marker.bindPopup(
    `<b>Obstacle</b><br/>${record.latitude}, ${record.longitude}`
  );
  // @ts-ignore
  marker.alt = record.id;

  marker.on("dragend", async (event) => {
    await fetch("/api/UpdateObstacle", {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify({
        id: event.target.alt,
        latitude: event.target.getLatLng().lat,
        longitude: event.target.getLatLng().lng,
      }),
    });
  });
}

/**
 * The function adds obstacle markers to the map.
 * @param map The map to add the obstacle markers to.
 */
export async function AddObstaclesMapBehavior(map: L.Map) {
  // Get the obstacles from the database
  const response = await fetch("/api/GetObstacles");
  // Convert the response to JSON
  records = await response.json();
  // Add a marker for each obstacle
  records.forEach((record) => {
    AddObstacle(record, map);
  });
  // Update the obstacles every 10 seconds
  setInterval(async () => {
    markers.forEach((marker) => {
      map.removeLayer(marker);
      marker = null;
    });
    const response = await fetch("/api/GetObstacles");
    records = await response.json();
    records.forEach((record) => {
      AddObstacle(record, map);
    });
  }, 10000);
}
