// author: Conner Ohnesorge (2023)
import { getXataClient } from "../../xata";
import type { APIRoute } from "astro";

/**
 * Handles the POST request for searching orders.
 * @param {APIRequest} request - The API request object.
 * @returns {Promise<Response>} - The response containing the search results or an error message.
 * @example
 * ```astro
 * import { POST } from "./SearchOrders.ts";
 * const response = await POST(context);
 * const { records } = await response.json();
 * ```
 *
 * ```ts
 * const response = await fetch("/api/SearchOrders.ts", {
 * method: "POST",
 * body: JSON.stringify({
 * query: "some query",
 * }),
 * });
 * const { records } = await response.json();
 * console.log(records);
 * ```
 *
 */
export const POST: APIRoute = async ({ request }) => {
  try {
    // Get the query from the request body
    const { query } = await request.json();

    // Get the Xata client
    const xata = getXataClient();

    // Search the orders table for the query
    const records = await xata.search.all(query, {
      tables: [{ table: "orders" }],
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
