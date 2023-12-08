// author: Conner Ohnesorge (2023) | conneroh.com

/**
 * Get the number of orders
 * @returns {Promise<number>} num
 **/
const GetClientsNumOrders = async (): Promise<number> => {
  // Call to the server to get the number of goods
  const num = await fetch("/api/GetNumGoods", { method: "GET" })
    .then((res) => res.json())
    .then((data) => data.num);
  return num;
};

// Export the function
export default GetClientsNumOrders;
