// author: Conner Ohnesorge (2023)
import { getXataClient } from "../../xata";
import type { APIRoute } from "astro";

/**
 * @type {APIRoute} Post Roomba
 * Handles the POST request for creating a new Roomba record.
 * @param {Request} request - The incoming request object.
 * @returns {Promise<Response>} - The response object.
 * @example
 * ```astro
 * import { POST } from "./PostRoomba.ts";
 * const response = await POST(context);
 * const { record } = await reponse.json();
 * ```
 *
 * ```ts
 * const response = await fetch("/api/PostRoomba.ts", {
 *  method: "POST",
 *     body: JSON.stringify({
 *         lat: 123,
 *         long: 123,
 *         deg: 123,
 *     }),
 * });
 * const { record } = await response.json();
 * console.log(record);
 * ```
 */
export const POST: APIRoute = async ({ request }): Promise<Response> => {
  try {
    // Get the latitude, longitude, and degrees from the request body
    const { lat, long, deg } = await request.json();

    // Get the Xata client
    const xata = getXataClient();

    // Create the record
    const record = await xata.db.roomba.create({
      latitude: lat,
      longitude: long,
      degrees: deg,
    });

    // Return the response
    return new Response(JSON.stringify(record), { status: 200 });
  } catch (error) {
    // Return the error response
    return new Response(error.message, { status: 500 });
  }
};
