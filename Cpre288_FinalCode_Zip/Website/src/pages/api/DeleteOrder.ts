// author: Conner Ohnesorge (2023)

import type { APIRoute } from "astro";
import { getXataClient } from "../../xata";

/*
 * @type {APIRoute} Delete All Plotters
 * @returns {Response} The Roomba Path
 * @description This is the API Route for deleting all plotters
 * @example
 * ```ts
 * import { POST } from "./DeletePlotters.ts";
 * const response = await POST(request);
 * const data = await response.json();
 * ```
 */
export const POST: APIRoute = async () => {

  try {

    // Sleep for 20 second
    // Get the Xata client
    const xata = getXataClient();
    
    // Select all plotters
    const records = await xata.db.orders.select(["id"]).getAll();

    // Delete all plotters
    records.forEach(async (element) => {
      await xata.db.orders.delete(element.id);
    });

    // Return a success response
    return new Response("success", { status: 201 });
  } catch (e: any) {
    // Return an error response
    return new Response(e.message, { status: 500 });
  }
};
