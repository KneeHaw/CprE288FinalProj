// author: Conner Ohnesorge (2023)
import type { APIRoute } from "astro";
import { getXataClient } from "../../xata";

/**
 * Handles the POST request to update a house's location.
 * @param request - The incoming request object.
 * @returns A response indicating the success or failure of the update operation.
 * @example
 * ```astro
 * import { POST } from "./UpdateHouse.ts";
 * const response = await POST(context);
 * const { record } = await response.json();
 * ```
 *
 * ```ts
 * const response = await fetch("/api/UpdateHouse.ts", {
 * method: "POST",
 * body: JSON.stringify({
 * id: "some id",
 * latitude: 123,
 * longitude: 123,
 * }),
 * });
 * const { record } = await response.json();
 * console.log(record);
 * ```
 *
 */
export const POST: APIRoute = async ({ request }) => {
  try {
    // get the body of the request
    const body = await request.json();
    // get the id of the plotter to update
    const id = body.id;

    const { lat, lng } = { lat: body.latitude, lng: body.longitude };

    const xata = getXataClient();

    await xata.db.houses.update(id, {
      latitude: lat,
      longitude: lng,
    });

    return new Response("success", { status: 200 });
  } catch (error) {
    return new Response(error.message, { status: 500 });
  }
};
