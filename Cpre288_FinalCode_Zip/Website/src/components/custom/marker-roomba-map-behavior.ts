// author: Conner Ohnesorge (2023) | conneroh.com

// Import the Leaflet library
import * as L from "leaflet";
// Import the Leaflet.marker.slideto library
import "leaflet.marker.slideto";
// Import the Leaflet.rotatedMarker library
import "leaflet-rotatedmarker";
// Import the Leaflet.fullscreen library
import "leaflet.fullscreen";
// Import the map icons from map-icons.ts
import { botIcon } from "./map-icons";

let roombaPath: L.Polyline;
let Lmap: L.Map;
let roombaMarker: L.Marker;
let currentDeg: number;
let desiredDeg: number;
let isMarkerAdded = false;

export async function addRoombaMapBehavior(map) {
  Lmap = map;
  setInterval(updateRoombaPosition, 100);
  await addRoombaPath();
}

/**
 * Updates the position of the Roomba marker on the map.
 * Fetches the Roomba data from the server and updates the marker's position and rotation.
 */
async function updateRoombaPosition() {
  const response = await fetch("/api/GetRoomba");
  if (response.status === 200) {
    const record = await response.json();
    if (!isMarkerAdded) {
      roombaMarker = L.marker([record.latitude, record.longitude], {
        icon: botIcon,
        rotationAngle: record.degrees,
        rotationOrigin: "center center",
      } as L.MarkerOptions & { rotationAngle?: number }).addTo(Lmap);
      isMarkerAdded = true;
      roombaMarker.bindPopup("Roomba: Battery: " + 55 + "%");
    }

    desiredDeg = record.degrees;
    // @ts-ignore
    roombaMarker.slideTo([record.latitude, record.longitude], {
      duration: 800,
    });

    // @ts-ignore
    roombaMarker.setRotationAngle(desiredDeg);
  }
}

/**
 * Fetches the Roomba path from the server and adds a polyline to the map.
 * @returns {Promise<void>} A promise that resolves when the Roomba path is added to the map.
 */
async function addRoombaPath() {
  try {
    const response = await fetch("/api/GetPath");
    if (response.status === 311) {
      return;
    }
    const records = await response.json();
    let latlngs = [];
    records.forEach((record) => {
      latlngs.push([record.latitude, record.longitude]);
    });
    // add a polyline to the map
    roombaPath = L.polyline(latlngs, { color: "red" }).addTo(Lmap);
  } catch (error) {
    console.log(error);
  }
}
