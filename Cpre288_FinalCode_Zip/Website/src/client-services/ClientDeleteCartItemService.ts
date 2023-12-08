// author: Conner Ohnesorge (2023) | conneroh.com
/**
 * Deletes a cart item by its id.
 * @param {string} id - The id of the cart item to delete.
 * @returns {Promise<any>} The response data from the server.
 * @throws {Error} If there is an error deleting the cart item.
 **/
const deleteCartItem = async (id: string): Promise<any> => {
  // Call to the server to delete the cart item
  const response = await fetch("/api/DeleteCartItem", {
    method: "POST",
    body: JSON.stringify({ id }),
    headers: {
      "content-type": "application/json",
    },
  });

  // Return the response if it is ok
  // Else throw an error
  if (!response.ok) {
    throw new Error(await response.text());
  }

  return await response.json();
};

// Export the function
export default deleteCartItem;
