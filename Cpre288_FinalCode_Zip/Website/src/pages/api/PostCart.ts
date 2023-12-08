// author: Conner Ohnesorge (2023)

// This file is used to handle the API route for posting the order to the database
import type { APIRoute } from "astro";
// Import the getXataClient function from the xata file
import { getXataClient } from "../../xata";

// This function posts the order to the database
export const POST: APIRoute = async ({ clientAddress: ipAddress, request }) => {
  try {
    // Get the xata client for interacting with the database
    const xata = getXataClient();

    // Get the request body as json (the latitude and longitude)
    const { good: good } = await request.json();

    const client = await xata.db.clients
      .filter({ ipaddress: ipAddress })
      .getFirst();

    // Create the record in the orders table and return the created record
    const record = await xata.db.carts.create({
      good: good,
      client: client,
    });

    // return the created record
    return new Response(JSON.stringify(record), { status: 201 });
  } catch (error) {
    // log the error
    console.error("PostOrder.ts: Error Posting the Order: ", error);
    // return the error response
    return new Response(error, { status: 500 });
  }
};
