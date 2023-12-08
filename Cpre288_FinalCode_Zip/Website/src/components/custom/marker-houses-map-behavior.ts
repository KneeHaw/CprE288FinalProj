// author: Conner Ohnesorge (2023) | conneroh.com
import * as L from "leaflet";
import { houseIcon } from "./map-icons";

// The map to add the markers to
let Lmap: L.Map | L.LayerGroup<any> = L.layerGroup();

/**
 * The function adds house markers to the map.
 * @param map The map to add the house markers to.
 */
export async function addHousesMarkers(map: L.Map | L.LayerGroup<any>) {
  // Set the map to the global variable
  Lmap = map;

  const response = await fetch("/api/GetHouses");
  const records = await response.json();
  records.forEach((record: any) => {
    const marker = L.marker([record.latitude, record.longitude], {
      icon: houseIcon,
    });
    marker.bindPopup(
      `<div class="popup"><h4>${record.name}</h4><p>${record.character}</p></div>`,
      { maxWidth: 200 }
    );
    // @ts-ignore
    marker.alt = record.id;
    marker.on("dragend", async (e) => {
      fetch("/api/UpdateHouse", {
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
    });
    marker.addTo(map);
  });
}

/**
 * The function zooms to the coordinates.
 * @param latitude The latitude to zoom to.
 * @param longitude The longitude to zoom to.
 */
export async function ZoomToCoords(latitude: number, longitude: number) {
  // @ts-ignore
  Lmap.setView([latitude, longitude], 18);
}
