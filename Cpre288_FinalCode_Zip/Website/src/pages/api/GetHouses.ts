// author: Conner Ohnesorge (2023)
import type { APIRoute } from "astro";
import { getXataClient } from "../../xata";

/*
 * @type {APIRoute} Get Houses
 * @returns {Response} The Houses
 * @description This is the API Route for getting all houses
 * @example
 * ```astro
 * import { GET } from "./GetHouses.ts";
 * const response = await GET(request);
 * const data = await response.json();
 * ```
 *
 * ```ts
 * const response = await fetch("/api/GetHouses", { method: "GET" });
 * const date = await response.json();
 * ```
 */
export const GET: APIRoute = async ({ request }) => {
  try {
    // Get the Xata client
    const xata = getXataClient();

    // Select all houses
    const records = await xata.db.houses
      .select(["id", "latitude", "longitude", "name", "character"])
      .getAll();

    // Return a success response
    return new Response(JSON.stringify(records), { status: 200 });
  } catch (error) {
    // Return an error response
    return new Response(JSON.stringify({ error: error.message }), {
      status: 500,
    });
  }
};
