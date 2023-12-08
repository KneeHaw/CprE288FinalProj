// author: Conner Ohnesorge (2023)
import { getXataClient } from "../../xata";
import type { APIRoute } from "astro";

/**
 * Handles the POST request for searching client records.
 * @param {Object} params - The request parameters.
 * @param {string} params.clientAddress - The client's IP address.
 * @returns {Promise<Response>} - The response containing the search result.
 * @throws {Error} - The error thrown by the Xata client.This is returned as a 500 error with the error message as the body.
 * @example
 * ```astro
 * import { POST } from "./SearchClient.ts";
 * const response = await POST(request);
 * const data = await response.json();
 * ```
 *
 * ```ts
 * import { POST } from "./SearchClient.ts";
 * const response = await POST(request);
 * const data = await response.json();
 * console.log(data);
 * ```
 **/
export const POST: APIRoute = async ({
  clientAddress: ipAddress,
}: {
  clientAddress: string;
}): Promise<Response> => {
  try {
    const xata = getXataClient();

    const records = await xata.search.all(ipAddress, {
      tables: [
        { table: "roomba", target: [] },
        { table: "orders", target: [] },
        { table: "plotter", target: [] },
        { table: "goods", target: [] },
        { table: "businesses", target: [] },
        { table: "houses", target: [] },
        { table: "obstacles", target: [] },
        { table: "plan", target: [] },
        {
          table: "clients",
          target: [
            { column: "ipaddress" },
            { column: "house" },
            { column: "xata.createdAt" },
            { column: "xata.updatedAt" },
          ],
        },
      ],
      fuzziness: 0,
      prefix: "phrase",
    });

    // return the first record
    return new Response(JSON.stringify(records[0]), { status: 200 });
  } catch (error) {
    return new Response(error.message, { status: 500 });
  }
};
