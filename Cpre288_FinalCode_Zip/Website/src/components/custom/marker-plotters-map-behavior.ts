// author: Conner Ohnesorge (2023) | conneroh.com

// Import the Leaflet library
import * as L from "leaflet";

// The array of markers used in the map.
export let markers: L.Marker[] = [];

/**
 * Adds a plotter marker to the map.
 *
 * @param record - The record containing latitude and longitude information.
 * @param map - The map to add the marker to.
 */
function AddPlotter(record, map) {
  // create a marker at the plotter's position
  const marker = L.marker([record.latitude, record.longitude], {
    draggable: true,
  });
  // bind the popup to the marker that shows the latitude and longitude
  marker.bindPopup(
    `Latitude: ${record.latitude}<br>Longitude: ${record.longitude}`
  );
  // @ts-ignore
  marker.alt = record.id;
  // update the plotter's position in the database when the marker is dragged
  marker.on("dragend", async (e) => {
    // update the plotter's position in the database
    await fetch("/api/UpdatePlotter", {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify({
        id: e.target.alt,
        latitude: e.target.getLatLng().lat,
        longitude: e.target.getLatLng().lng,
      }),
    });
    // bind the popup to the marker that shows the latitude and longitude
    marker.bindPopup(
      `Latitude: ${e.target.getLatLng().lat}<br>Longitude: ${e.target.getLatLng().lng
      }`
    );
  });
  // add the markers to the map
  marker.addTo(map);
  // add the marker to the markers array
  markers.push(marker);
}

/**
 * Adds the plotters map behavior to the map.
 * @param map - The map to add the behavior to.
 **/
export async function addPlottersMapBehavior(map: L.Map) {
  // fetch from the /api/GetPlotters endpoint
  const response = await fetch("/api/GetPlotters");
  // convert the response to JSON
  const records = await response.json();
  // add the markers to the map
  records.forEach((record: any) => {
    AddPlotter(record, map);
  });
}
