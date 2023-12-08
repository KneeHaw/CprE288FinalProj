// author: Conner Ohnesorge (2023) | conneroh.com
import * as L from "leaflet";

import { stopSignIcon } from "./map-icons";

/**
 * Adds a stop sign marker to the map.
 *
 * @param record - The record containing the latitude and longitude of the stop sign.
 * @param map - The map to add the marker to.
 */
function AddStopSign(record, map: L.Map) {
  const stopSignMarker = L.marker([record.latitude, record.longitude], {
    icon: stopSignIcon,
  }).addTo(map);
  // @ts-ignore
  stopSignMarker.alt = record.id;
  stopSignMarker.on("dragend", async (event) => {
    await fetch("/api/UpdateStopSigns", {
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
 * The function adds stop sign markers to the map.
 * @param map - The map to add the stop sign markers to.
 */
export async function addStopSignMapBehavior(map: L.Map) {
  const response = await fetch("/api/GetStopSigns");
  const records = await response.json();
  records.forEach((record) => {
    AddStopSign(record, map);
  });
}
