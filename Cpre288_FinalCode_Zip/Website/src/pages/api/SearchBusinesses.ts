// author: Conner Ohnesorge (2023)
import { getXataClient } from "../../xata";
import type { APIRoute } from "astro";

/**
 * @type {APIRoute} Search Businesses
 * Handles the POST request for searching businesses.
 * @param {APIRequest} request - The API request object.
 * @returns {Promise<Response>} - The response object.
 */
export const POST: APIRoute = async ({ request }) => {
  try {
    // Get the query from the request body
    const { query } = await request.json();
    // Get the Xata client
    const xata = getXataClient();
    // Search the businesses table for the query
    const records = await xata.search.all(query, {
      tables: [{ table: "businesses" }],
      fuzziness: 2,
      prefix: "phrase",
    });
    // Return the response
    return new Response(JSON.stringify(records), { status: 200 });
  } catch (error) {
    // Return the error response
    return new Response(error.message, { status: 500 });
  }
};
