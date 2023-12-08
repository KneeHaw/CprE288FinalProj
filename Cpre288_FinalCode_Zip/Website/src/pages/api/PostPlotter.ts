// author: Conner Ohnesorge (2023)
import type { APIRoute } from "astro";

import { getXataClient } from "../../xata";

/**
 * @typedef {import("astro").APIRouteContext} APIRouteContext
 * @param {APIRouteContext} context - The API route context containing the request and response objects.
 * @returns {Promise<Response>} - The response containing the created record or an error message.
 * @example
 * ```astro
 * import { POST } from "./PostPlotter.ts";
 * const response = await POST(context);
 * const { record } = await response.json();
 * ```
 *
 * ```ts
 * const response = await fetch("/api/PostPlotter", {
 *  method: "POST",
 *   body: JSON.stringify({
 *    lat: 123,
 *   lng: 123,
 * }),
 * });
 * const { record } = await response.json();
 * console.log(record);
 * ```
 **/
export const POST: APIRoute = async ({ request }) => {
  try {
    // Get the latitude and longitude from the request
    const { lat, lng } = await request.json();
    // get the the xata client
    const xata = getXataClient();
    // Create a new plotter at teh latitude and longitude
    const record = await xata.db.plotter.create({
      latitude: lat,
      longitude: lng,
    });
    // return a new response
    return new Response(JSON.stringify(record), { status: 201 });
  } catch (error) {
    // return an error
    return new Response(error.message, { status: 500 });
  }
};
