<script lang="ts">
  // author: Conner Ohnesorge (2023) | conneroh.com
  import { onMount } from "svelte";
  let data = [];
  let selectedHouse = null;

  /**
   * Toggles if the checkout house select modal is shown
   **/
  async function toggleShownModal() {
    const response = await fetch("/api/GetNumGoods");
    const data = await response.json();
    if (data.num > 0) {
      const modal = document.getElementById("static-modal");
      modal.classList.toggle("hidden");
    } else {
      alert("You need to have items in your cart first");
    }
  }

  /**
   * Hides the checkout house select modal
   **/
  function hideModal() {
    const modal = document.getElementById("static-modal");
    modal.classList.add("hidden");
  }

  /**
   * Runs on mounting of the component
   **/
  onMount(onMountActions);

  /**
   * @typedef {Object} House
   * Gets the houses from the server on mount
   **/
  function onMountActions() {
    getHouses();
  }
  /**
   * Gets the houses from the server
   **/
  async function getHouses() {
    const response = await fetch("/api/GetHouses", {
      method: "GET",
    });
    data = await response.json();
  }

  async function checkoutOrder() {
    if (selectedHouse == null) {
      alert("You need to select a house");
    }

    const goodsResponse = await fetch("/api/GetCart", {
      method: "GET",
    });
    const goodsData = await goodsResponse.json();
    const goodsIds = goodsData.map((good) => good.good.id);
    await fetch("/api/PostOrder", {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify({
        house: selectedHouse,
        goods: [...goodsIds],
      }),
    });

    // remove all the goods from the cart using DeleteCartItem
    goodsIds.forEach(async (goodId) => {
      await fetch("/api/DeleteCartItem", {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify({
          id: goodId,
        }),
      });
    });

    hideModal();

    await fetch("/api/DeleteCartItems", {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
    });
    await new Promise((resolve) => setTimeout(resolve, 5000));

    await fetch("/api/DeleteOrder", {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
    });
  }

  function setSelectedVisual(id: string) {
    // set allthe non selectedhouses to jst the card
    const allHouses = document.querySelectorAll("[data-v0-t='card']");
    allHouses.forEach((house) => {
      if (house.id != id) {
        house.classList.remove("bg-red-400");
      }
    });
    // Set the background color of the selected house to red
    const selectedHouse = document.getElementById(id);
    selectedHouse.classList.add("bg-red-400");
  }
</script>

<button
  data-modal-toggle="static-modal"
  class="block text-white bg-red-700 hover:bg-red-800 focus:ring-4 focus:outline-none focus:ring-red-300 font-medium rounded-lg text-sm px-5 text-center dark:bg-red-600 dark:hover:bg-red-700 dark:focus:ring-red-800"
  type="button"
  on:click={toggleShownModal}
>
  Checkout!
</button>

<div class="bg-red-800">
  <div
    id="static-modal"
    tabindex="-1"
    class="hidden houseselect absolute right-0 left-0 justify-center items-center bg-red-800 pl-20"
  >
    <div class="absolute mt-30">
      <!-- Modal content -->
      <div class="rounded-lg shadow">
        <!-- Modal header -->

        <div class="flex flex-row">
          <h2
            class="text-2xl p-4 font-semibold text-gray-200 dark:text-gray-200"
          >
            Select a house
          </h2>
          <div
            class="flex items-end justify-end p-4 md:p-5 border-b rounded-t dark:border-gray-600"
          >
            <button
              type="button"
              class="text-gray-400 bg-transparent hover:bg-gray-200 hover:text-gray-900 rounded-lg text-sm h-8 ms-auto inline-flex justify-center items-center dark:hover:bg-gray-600 dark:hover:text-white"
              data-modal-hide="static-modal"
              on:click={hideModal}
            >
              <svg
                class="w-3 h-3"
                aria-hidden="true"
                xmlns="http://www.w3.org/2000/svg"
                fill="none"
                viewBox="0 0 14 14"
              >
                <path
                  stroke="currentColor"
                  stroke-linecap="round"
                  stroke-linejoin="round"
                  stroke-width="2"
                  d="m1 1 6 6m0 0 6 6M7 7l6-6M7 7l-6 6"
                />
              </svg>
              <span class="sr-only">Close modal</span>
            </button>
          </div>
        </div>

        <div class="p-5">
          {#each data as record}
            <div class="housecard">
              <div
                role="button"
                aria-label="card"
                tabindex="0"
                aria-roledescription="card"
                id={record.id}
                class="rounded-lg border bg-card text-card-foreground shadow-sm flex items-center space-x-4"
                data-v0-t="card"
                on:keydown={() => {
                  selectedHouse = record.id;
                  setSelectedVisual(record.id);
                }}
                on:click={() => {
                  selectedHouse = record.id;
                  setSelectedVisual(record.id);
                }}
              >
                <label for={record.id} class="text-sm"
                  >House: {record.name}</label
                >
                <span class="relative flex rounded-full w-12 h-12"> </span>
                <p class="text-sm">
                  {parseInt(record.latitude)}, {parseInt(record.longitude)}
                </p>
              </div>
            </div>
          {/each}
        </div>
        <!-- Modal footer -->
        <div
          class="flex items-center p-4 md:p-5 border-t border-gray-200 rounded-b dark:border-gray-600"
        >
          <button
            data-modal-hide="static-modal"
            type="button"
            class="text-white bg-blue-700 hover:bg-blue-800 focus:ring-4 focus:outline-none focus:ring-blue-300 font-medium rounded-lg text-sm px-5 py-2.5 text-center dark:bg-blue-600 dark:hover:bg-blue-700 dark:focus:ring-blue-800"
            on:click={checkoutOrder}>Yes</button
          >
          <button
            data-modal-hide="static-modal"
            on:click={hideModal}
            type="button"
            class="ms-3 text-gray-500 bg-white hover:bg-gray-100 focus:ring-4 focus:outline-none focus:ring-blue-300 rounded-lg border border-gray-200 text-sm font-medium px-5 py-2.5 hover:text-gray-900 focus:z-10 dark:bg-gray-700 dark:text-gray-300 dark:border-gray-500 dark:hover:text-white dark:hover:bg-gray-600 dark:focus:ring-gray-600"
            >Cancel</button
          >
        </div>
      </div>
    </div>
  </div>
</div>

<style>
  .housecard {
    background: var(--gray-999);
  }

  .mt-30 {
    background: var(--gray-999) !important;
  }
</style>
