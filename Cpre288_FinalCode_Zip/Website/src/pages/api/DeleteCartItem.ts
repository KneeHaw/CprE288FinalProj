// author: Conner Ohnesorge (2023)
import type {APIRoute} from "astro";
import {getXataClient} from "../../xata";

/**
 * @type {APIRoute}
 * Handles the POST request to delete a cart item.
 * @param {APIRequest} request - The request object containing the request details.
 * @returns {Promise<Response>} A promise that resolves to a success response if the cart item is deleted successfully,
 * or an error response if there was an issue deleting the cart item.
 */
export const POST: APIRoute = async ({request}) => {
    try {
        // Get the Xata client
        const xata = getXataClient();
        
        // Get the id from the request body
        const {id} = await request.json();
        
        // Delete the order
        await xata.db.carts.delete(id);
        
        // Return a success response
        return new Response("success", {status: 201});
    } catch (e: any) {
        // Return an error response
        return new Response(e.message, {status: 500});
    }
};
