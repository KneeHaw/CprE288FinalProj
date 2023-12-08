// author: Conner Ohnesorge (2023)
import type { APIRoute } from "astro";
import { getXataClient } from "../../xata";

/**
 * @type {APIRoute} Get The Plotters
 * @returns {Response} The Response Object with the Plotters from the Database
 * @throws {Response} The Response Object with the Error Message
 * @description This function gets the Plotters from the Database
 * @example
 * ```astro
 * import { GET } from "./GetPlotters.ts";
 * const response = await GET(request);
 * const data = await response.json();
 * ```
 * ```ts
 * import { GET } from "./GetPlotters.ts";
 * const response = await GET(request);
 * const data = await response.json();
 * ```
 */
export const GET: APIRoute = async (): Promise<Response> => {
  try {
    // Get the Xata Client
    const xata = getXataClient();
    // Get the Records
    const records = await xata.db.plotter
      .select(["latitude", "longitude"])
      .getAll();
    // Return the Response
    return new Response(JSON.stringify(records), { status: 200 });
  } catch (error) {
    console.error("GetPlotters.ts: Error Getting the Plotters");
    console.error(error);
    // Return the Error Response
    return new Response(error.message, { status: 500 });
  }
};
