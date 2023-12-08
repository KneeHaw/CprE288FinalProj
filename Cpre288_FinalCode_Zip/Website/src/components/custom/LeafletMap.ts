// author: Conner Ohnesorge (2023)
import L from "leaflet";
import mapImage from "../../assets/map.png";
import { addHousesMarkers } from "./marker-houses-map-behavior";
import { addSidebarMapBehavior } from "./sidebar-map-behavior";
import { addPlottersMapBehavior } from "./marker-plotters-map-behavior";
import { addRoombaMapBehavior } from "./marker-roomba-map-behavior";
import "leaflet.fullscreen";
import { addStopSignMapBehavior } from "./marker-stop-sign-map-behavior";
import { AddObstaclesMapBehavior } from "./marker-obstacles-map-behavior.ts";

export interface Props {
  containerstyle: string;
}

/**
 * Represents a custom Leaflet map element.
 */
export class LeafletMap extends HTMLElement {
  /**
   * Constructs a new instance of the LeafletMap class.
   */
  constructor() {
    super();

    const bounds = [
      [0, 0], //padding
      [926, 679], // image size
    ];

    /**
     * Represents the options for the Leaflet map.
     */
    interface MapOptions extends L.MapOptions {
      fullscreenControl?: boolean;
    }

    const map = L.map("map", {
      crs: L.CRS.Simple,
      maxZoom: 10,
      minZoom: -1.25,
      maxBounds: [
        [0, 0],
        [926, 679],
      ],
      attributionControl: false,
      fullscreenControl: true,
      fullscreenControlOptions: {
        position: "bottomright",
      },
    } as MapOptions);

    L.imageOverlay(mapImage.src, [
      [0, 0],
      [926, 679],
    ]).addTo(map);

    map.fitBounds([
      [0, 0],
      [926, 679],
    ]);
    map.setZoom(-1);
    this.addMapBehaviors(map).then();
  }

  /**
   * Adds various behaviors to the Leaflet map.
   * @param map - The Leaflet map instance.
   */
  async addMapBehaviors(map: L.Map) {
    addSidebarMapBehavior(map);
    await addHousesMarkers(map);
    await addPlottersMapBehavior(map);
    await addRoombaMapBehavior(map);
    await addStopSignMapBehavior(map);
    await AddObstaclesMapBehavior(map);
  }
}
