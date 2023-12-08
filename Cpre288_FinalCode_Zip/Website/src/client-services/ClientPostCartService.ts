// author: Conner Ohnesorge (2023) | conneroh.com
/**
 * Posts a new good to the cart on the server.
 * @param {string} good - The good to add to the cart.
 * @param {string} clientAddress - The IP address of the client.
 * @returns {Promise<any>} The response data from the server.
 * @throws {Error} If there is an error posting the good to the cart.
 */
const postCart = async (good: string, clientAddress: string): Promise<any> => {
  const response = await fetch("/api/PostCart", {
    method: "POST",
    body: JSON.stringify({ good, clientAddress }),
    headers: {
      "content-type": "application/json",
    },
  });

  if (!response.ok) {
    throw new Error(await response.text());
  }

  return await response.json();
};

// Export the function
export default postCart;
