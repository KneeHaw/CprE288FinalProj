// author: Conner Ohnesorge (2023)

import type { APIRoute } from "astro";
import { getXataClient } from "../../xata";

/**
 * @type {APIRoute} Get The Roomba Path
 * @returns {Response} The Roomba Path
 * @description This is the API Route for getting the Roomba Path
 * @example
 * ```ts
 * import { GET } from "./GetPath.ts";
 * const response = await GET(request);
 * const data = await response.json();
 * ```
 **/
export const GET: APIRoute = async (): Promise<Response> => {
  try {
    // Get the Xata Client
    const xata = getXataClient();

    // Get the Roomba Path
    const records = await xata.db.roomba.select([
      "id",
      "latitude",
      "longitude",
      "degrees"
    ]).getAll();

    if (records.length === 0 || records === undefined) {
      return new Response("No Roomba Records Found", { status: 311 });
    }
    return new Response(JSON.stringify(records), { status: 200 });
  }
  catch (error) {
    return new Response("Roomba Rush Encountered an Error: " + error.message, { status: 500 })
  }

};
