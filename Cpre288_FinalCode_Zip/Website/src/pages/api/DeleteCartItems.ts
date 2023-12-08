// author: Conner Ohnesorge (2023)

import type { APIContext, APIRoute } from "astro";
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
export const POST: APIRoute = async ({ clientAddress }: APIContext) => {
    try {
        // Sleep for 20 second
        // Get the Xata client
        const xata = getXataClient();


        const client = await xata.db.clients
            .filter({ ipaddress: clientAddress })
            .getFirst();
        // Select all plotters
        const records = await xata.db.carts.select(["id"]).filter({ client: client.id }).getAll();

        // Delete all plotters
        records.forEach(async (element) => {
            await xata.db.carts.delete(element.id);
        });

        // Return a success response
        return new Response("success", { status: 201 });
    } catch (e: any) {
        // Return an error response
        return new Response(e.message, { status: 500 });
    }
};
