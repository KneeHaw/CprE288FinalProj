// author: Conner Ohnesorge (2023)
import type { APIRoute } from "astro";
import { getXataClient } from "../../xata";

/**
 * Handles the POST request to update a business.
 * @param {APIRequest} request - The API request object.
 * @returns {Promise<Response>} - A promise that resolves to a response indicating the success or failure of the update.
 * @example
 * ```astro
 * import { POST } from "./UpdateBusiness.ts";
 * const response = await POST(context);
 * const { record } = await response.json();
 * ```
 *
 * ```ts
 * const response = await fetch("/api/UpdateBusiness.ts", {
 *  method: "POST",
 *    body: JSON.stringify({
 *       id: "some id",
 *      latitude: 123,
 *     longitude: 123,
 * }),
 * });
 * const { record } = await response.json();
 * console.log(record);
 * ```
 */
export const POST: APIRoute = async ({ request }) => {
  try {
    // get the body of the request
    const body = await request.json();
    // get the id of the plotter to update
    const { lat, lng, id } = {
      lat: body.latitude,
      lng: body.longitude,
      id: body.id,
    };
    // get the xata client
    const xata = getXataClient();
    // update the plotter
    await xata.db.businesses.update(id, {
      latitude: lat,
      longitude: lng,
    });

    // return a success response
    return new Response("succsess", { status: 200 });
  } catch (error) {
    // return an error response
    return new Response(error.message, { status: 500 });
  }
};
