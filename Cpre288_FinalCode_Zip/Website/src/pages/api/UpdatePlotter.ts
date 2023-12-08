// author: Conner Ohnesorge (2023) | conneroh.com
import type { APIContext, APIRoute } from "astro";
import { getXataClient } from "../../xata";

/**
 * Handles the POST request to update a plotter's location.
 * @param {APIRouteRequest} request - The request object containing the body of the request.
 * @returns {Promise<Response>} - A promise that resolves to a success response or an error response.
 */
export const POST: APIRoute = async ({ request }: APIContext): Promise<Response> => {
  try {
    // get the body of the request
    const body = await request.json();
    
    // get the id of the plotter to update
    const id = body.id;
    
    // get the latitude and longitude from the body
    const { lat, lng } = { lat: body.latitude, lng: body.longitude };
    
    // get the xata client
    const xata = getXataClient();
    
    // update the plotter
    await xata.db.plotter.update(id, {
      latitude: lat,
      longitude: lng,
    });

    // return a success response
    return new Response("success", { status: 200 });
  } catch (error) {
    // return an error response
    return new Response(error.message, { status: 500 });
  }
};
