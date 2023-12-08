// author: Conner Ohnesorge (2023)
import type { APIContext, APIRoute } from "astro";
import { getXataClient } from "../../xata";

export const GET: APIRoute = async ({ clientAddress }: APIContext) => {
  try {
    // Get the Xata client
    const xata = getXataClient();

    // Select all orders
    const records = await xata.db.orders
      .select([
        "id",
        "goods",
        "house.id",
        "house.latitude",
        "house.longitude",
        "house.name",
        "client.id",
        "client.ipaddress",
        "client.name",
      ])
      .filter({
        "client.ipaddress": clientAddress,
      })
      .getAll();

    // Return a success response
    return new Response(JSON.stringify(records), { status: 200 });
  } catch (error) {
    // Return an error response
    return new Response(error.message, { status: 500 });
  }
};
