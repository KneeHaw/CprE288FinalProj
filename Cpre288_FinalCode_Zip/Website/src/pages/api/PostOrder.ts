// author: Conner Ohnesorge (2023)
import type { APIRoute } from "astro";

import { getXataClient } from '../../xata';

/*
 * @type {APIRoute} Post The Order
 * @param {APIContext} context The API Context
 * @returns {Response} The Response Object with the Order from the Database
 * @throws {Response} The Response Object with the Error Message
 * @description This function posts the Order to the Database
 * @example
 * ```astro
 * import { POST } from "./PostOrder.ts";
 * const clientAddress = "0.0.0.1";
 * const response = await POST( { clientAddress } );
 * const body = await response.json();
 * console.log(body);
 * ``` 
 * 
 * ```ts
 * const response = await fetch("/api/PostOrder");
 * const data = await response.json();
 * console.log(data);
 * ```
 */
export const POST: APIRoute = async ({ clientAddress: ipAddress, request }) => {
    try {
        // get the xata client
        const xata = getXataClient();
        // get the request body as json (the latitude and longitude)
        const { goods: reqGood, house: reqHouse } = await request.json();

        const client = await xata.db.clients.filter({ ipaddress: ipAddress }).getFirst();

        const record = await xata.db.orders.create({
          goods: reqGood,
          house: reqHouse,
          client: client.id,
        });

        // return the created record
        return new Response(JSON.stringify(record), { status: 201 });
    } catch (error) {
        // log the error
        console.error("PostOrder.ts: Error Posting the Order: ", error);
        // return the error response
        return new Response(error.message, { status: 500 });
    }
}
