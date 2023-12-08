// author: Conner Ohnesorge (2023) | conneroh.com
import type { Map } from "leaflet";

/**
 * Adds sidebar map behavior to the given map.
 * @param map - The map object.
 */
export function addSidebarMapBehavior(map: Map) {
  const menuItems = document.querySelectorAll(".menu-item");
  const sidebar = document.querySelector(".sidebar");
  const buttonClose = document.querySelector(".close-button");

  menuItems.forEach((item) => {
    item.addEventListener("click", (e) => {
      const target = e.target as HTMLElement;

      if (
        target.classList.contains("active-item") ||
        !document.querySelector(".active-sidebar")
      ) {
        document.body.classList.toggle("active-sidebar");
      }

      // show content
      // @ts-ignore
      showContent(target.dataset.item);
      // add active class to menu item
      addRemoveActiveItem(target, "active-item");
    });
  });

  /**
   * The function closes the sidebar when the close button is clicked.
   */
  buttonClose.addEventListener("click", () => {
    closeSidebar();
  });

  /**
   * The function adds the given class to the target element and removes the class from the element with the given class name.
   * @param target The target element to add the class to.
   * @param className The class name to add to the target element.
   * @returns The target element with the added class.
   */
  function addRemoveActiveItem(target: HTMLElement, className: string) {
    const element = document.querySelector(`.${className}`);
    target.classList.add(className);
    if (!element) return;
    element.classList.remove(className);
  }

  /**
   * Shows the content with the given data attribute.
   * @param dataContent - The data attribute of the content to show.
   */
  function showContent(dataContent: string) {
    const idItem = document.querySelector(`#${dataContent}`) as HTMLElement;
    addRemoveActiveItem(idItem, "active-content");
    map.dragging.disable();
    map.touchZoom.disable();
    map.doubleClickZoom.disable();
    map.scrollWheelZoom.disable();
    map.boxZoom.disable();
    map.keyboard.disable();
    if (map.tap) map.tap.disable();
    document.getElementById("map").style.cursor = "default";
  }

  document.addEventListener("keydown", function (event) {
    if (event.key === "Escape") {
      closeSidebar();
    }
  });

  // close sidebar when click outside
  document.addEventListener("click", (e) => {
    const target = e.target as HTMLElement;
    if (!target.closest(".sidebar")) {
      closeSidebar();
    }
  });

  /**
   * Closes the Sidebar Element.
   * @returns The sidebar element.
   */
  function closeSidebar() {
    document.body.classList.remove("active-sidebar");
    const element = document.querySelector(".active-item");
    const activeContent = document.querySelector(".active-content");
    if (!element) return;
    element.classList.remove("active-item");
    activeContent.classList.remove("active-content");
    map.dragging.enable();
    map.touchZoom.enable();
    map.doubleClickZoom.enable();
    map.scrollWheelZoom.enable();
    map.boxZoom.enable();
    map.keyboard.enable();
    if (map.tap) map.tap.enable();
    document.getElementById("map").style.cursor = "grab";
  }
}
