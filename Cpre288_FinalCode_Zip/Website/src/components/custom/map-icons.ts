// author: Conner Ohnesorge (2023) | conneroh.com
import L from "leaflet";
import botImg from "../../assets/bot.png";
import stopSignImg from "../../assets/stop-sign.png";
import houseImg from "../../assets/home.png";
import obstacleImg from "../../assets/cone.png";

/**
 * Represents the icon for the bot on the map.
 */
export const botIcon = L.icon({
  iconUrl: botImg.src,
  iconSize: [50, 50],
  iconAnchor: [25, 25],
  popupAnchor: [0, -25],
  shadowUrl: null,
  shadowSize: null,
  shadowAnchor: null,
  className: "roomba-icon marker-icon-rotate",
  // @ts-ignore
  rotationAngle: Math.random() * 360,
});

/**
 * Represents the stop sign icon used in the map.
 */
export const stopSignIcon = L.icon({
  iconUrl: stopSignImg.src,
  iconSize: [50, 50],
  iconAnchor: [25, 25],
  popupAnchor: [0, -25],
  shadowUrl: null,
  shadowSize: null,
  shadowAnchor: null,
  className: "roomba-icon marker-icon-rotate",
});

/**
 * Represents the obstacle icon used in the map.
 */
export const obstacleIcon = L.icon({
  iconUrl: obstacleImg.src,
  iconSize: [30, 30],
  iconAnchor: [25, 25],
  popupAnchor: [0, -25],
  shadowUrl: null,
  shadowSize: null,
  shadowAnchor: null,
  // @ts-ignore
  rotationAngle: Math.random() * 360,
  className: "roomba-icon marker-icon-rotate",
});

/**
 * Represents the house icon used in the map.
 */
export const houseIcon = L.icon({
  iconUrl: houseImg.src,
  iconSize: [30, 30],
  iconAnchor: [25, 25],
  popupAnchor: [0, -25],
  shadowSize: null,
  shadowAnchor: null,
  className: "roomba-icon marker-icon-rotate",
});
