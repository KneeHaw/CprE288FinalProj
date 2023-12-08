// author: Conner Ohnesorge (2023)

import type { APIContext, APIRoute } from "astro";
import { getXataClient } from "../../xata";

/*
 * @type {APIRoute} GET /api/orders
 */
export const POST: APIRoute = async ({ request, clientAddress }: APIContext) => {
  try {
    // Get the Xata client
    const xata = getXataClient();

    // Get the order ID from the request body
    const {id} = await request.json();

    // Get the order record
    const record = await xata.db.orders.read(id);

    // Return a success response
    return new Response(JSON.stringify(record), { status: 200 });
  } catch (error) {
    // Return an error response
    return new Response(error.message, { status: 500 });
  }
};
