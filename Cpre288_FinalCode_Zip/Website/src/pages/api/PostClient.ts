// author: Conner Ohnesorge (2023) | conneroh.com
import { getXataClient } from "../../xata";
import type { APIRoute } from "astro";
import type { APIContext } from "astro";

/**
 * @type {APIRoute} Post The Client
  * @param {APIContext} context The API Context
  * @returns {Response} The Response Object with the Client from the Database
  * @throws {Response} The Response Object with the Error Message
  * @description This function posts the Client to the Database
  * @example
  * ```ts
  *  const response = await fetch("/api/clients", {
  *  method: "POST",
  *  headers: {
  *  "Content-Type": "application/json",
  *  },
  *  });
  *  const data = await response.json();
  *  console.log(data);
  * ```
  **/
export const POST: APIRoute = async ({ clientAddress }: APIContext) => {
  try {
    // Get the Xata Client
    const xata = getXataClient();

    // Create the Record
    const record = await xata.db.clients.create({
      ipaddress: clientAddress,
    });

    // Return the created Record 
    return new Response(JSON.stringify(record), { status: 201 });
  } catch (error) {
    console.error("PostClient.ts: Error Posting the Client: ", error);
    // Return the Error Response
    return new Response(error.message, { status: 500 });
  }
};
