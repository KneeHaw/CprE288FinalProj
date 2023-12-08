// author: Conner Ohnesorge (2023) | conneroh.com

/**
 * The function gets the good with the given id.
 * @param id The id of the good to get.
 * @returns The good with the given id.
 **/
const GetGoodByIdService = async (id: string) => {
  // Call to the server to get the good by id
  const response = await fetch("/api/GetGoodById", {
    method: "POST",
    body: JSON.stringify({ id }),
    headers: {
      "content-type": "application/json",
    },
  });

  // Return the response
  return await response.json();
};

// Export the function
export default GetGoodByIdService;
