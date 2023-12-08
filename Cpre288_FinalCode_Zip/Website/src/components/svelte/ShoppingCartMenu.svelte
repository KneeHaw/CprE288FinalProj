<script lang="ts">
  // author: Conner Ohnesorge (2023) | conneroh.com
  import { Tooltip } from "flowbite-svelte";
  import { onMount } from "svelte";
  import CheckoutModal from "./Modals/CheckoutModal.svelte";

  let data = [];

  // Defines the Mount function (Running when the component is mounted)
  onMount(onMountActions);

  // Defines the onMountActions function
  function onMountActions() {
    setInterval(updateData, 1000);
    updateData();
  }

  /**
   * Checks if the object is iterable
   * @param obj The object to check
   */
  function isIterable(obj: any) {
    // checks for null and undefined
    if (obj == null) {
      return false;
    }
    return typeof obj[Symbol.iterator] === "function";
  }

  /**
   * Updates the data variable with the data from the server
   */
  async function updateData() {
    const response = await fetch("/api/GetCart", {
      method: "GET",
    });
    data = await response.json();
  }

  /**
   * Deletes the order from the server
   * @param e The event
   */
  async function delOrder(e: any) {
    // Call the server to delete the item from the cart
    await fetch("/api/DeleteCartItem", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ id: e.target.id }),
    });
    // Update the data variable
    await updateData();
  }
</script>

<!-- The Svelte HTML for the component, ShoppingCartMenu -->
<div class="p-5">
  {#if !isIterable(data) || data.length == 0}
    <div class="flex flex-col items-center justify-center h-full">
      <h1 class="text-3xl font-bold text-gray-700 dark:text-white">
        No Items in your cart
      </h1>
      <p class="text-gray-500 dark:text-gray-400">
        Add some items to your cart to see them here.
      </p>
    </div>
  {:else if isIterable(data)}
    <h2 class="items-center flex justify-center">
      <CheckoutModal />
    </h2>
    <div class="ordersList">
      {#each data as record}
        <div
          class="flex flex-row items-center border border-gray-200 rounded-lg shadow md:flex-row md:max-w-xl dark:border-gray-700 dark:bg-gray-800 dark:hover:bg-gray-50"
        >
          {#if record.good != null}
            <img
              class="object-cover rounded-t-lg w-20 md:w-32 lg:w-48 m-w-20 max-w-100 md:rounded-none md:rounded-s-lg"
              src={record.good.photo.url}
              alt=""
            />
          {/if}
          <div class="flex flex-col justify-between p-4 leading-normal">
            <h6
              class="mb-1 text-xl font-bold tracking-tight text-gray-900 dark:text-white"
            >
              {#if record.good != null}
                {record.good.name}
              {/if}
            </h6>
            <p class="mb-1 font-normal text-gray-700 dark:text-gray-400">
              {#if record.good != null}
                {record.good.calories} calories
              {/if}
            </p>
            <p class="mb-1 font-normal text-gray-700 dark:text-gray-400">
              {#if record.good != null}
                type: {record.good.type}
              {/if}
            </p>
            <div class="flex flex-row">
              <button
                on:click={delOrder}
                id={record.id}
                class=" px-4 py-2 mt-2 text-sm font-medium text-white bg-red-500 rounded-md hover:bg-red-300 focus:outline-none focus:bg-red-600"
              >
                Remove
              </button>
            </div>
          </div>
        </div>
      {/each}
    </div>
  {/if}
</div>

<style>
  .ordersList {
    overflow-y: auto;
    height: 100%;
    width: 100%;
    line-height: 1em;
    padding-bottom: 5em;
  }

  .ordersList::-webkit-scrollbar {
    -webkit-appearance: none;
  }

  .ordersList::-webkit-scrollbar:vertical {
    width: 24px;
  }

  .ordersList::-webkit-scrollbar:horizontal {
    height: 11px;
  }

  .ordersList::-webkit-scrollbar-thumb {
    border-radius: 8px;
    border: 1px solid white; /* should match background, can't be transparent */
    background-color: var(--gray-300);
  }
</style>
