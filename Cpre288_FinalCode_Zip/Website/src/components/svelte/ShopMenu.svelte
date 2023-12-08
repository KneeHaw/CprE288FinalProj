<script lang="ts">
  // author: Conner Ohnesorge (2023) | conneroh.com
  import { onMount } from "svelte";
  import { Confetti } from "svelte-confetti";

  // Duration of the confetti
  const confettiDuration = 5000;
  // Selected good variable initialized to 1
  let selectedGood = 1;
  // Celebrate variable initialized to false
  let celebrate: boolean = false;

  // Search variable initialized to empty string
  let search = "";
  // Data variable initialized to empty array
  let data = [];

  // Runs on mounting of the component
  async function onMountActions() {
    updateData();
    creatclient();
  }

  /**
   * Runs on mounting of the component
   **/
  onMount(onMountActions);


  /**
   * Updates the data in the table
   **/
  async function updateData() {
    const response = await fetch("/api/GetGoods", {
      method: "GET",
      headers: {
        "Content-Type": "application/json",
      },
    });
    data = await response.json();
    if (search != "") {
      data = data.filter((record) => {
        const nameMatches = record.name
          .toLowerCase()
          .includes(search.toLowerCase());
        const typeMatches = record.type
          .toLowerCase()
          .includes(search.toLowerCase());
        const providerMatches = record.provider.name
          .toLowerCase()
          .includes(search.toLowerCase());
        const caloriesMatches = record.calories.toString().includes(search);
        // use regex to match fuzzy name
        const fuzzyNameMatches = record.name
          .toLowerCase()
          .match(new RegExp(search.toLowerCase().split("").join(".*")));
        return (
          nameMatches ||
          typeMatches ||
          providerMatches ||
          caloriesMatches ||
          fuzzyNameMatches
        );
      });
    }
  }

  /**
   * Adds an order to the cart
   * @param e the event
   **/
  async function addorder(e: any) {
    celebrate = true;
    selectedGood = e.target.id;
    // Return the Create Cart Item Response
    return await fetch("/api/PostCart", {
      method: "POST",
      headers: {
        "content-type": "application/json",
      },
      body: JSON.stringify({
        good: selectedGood,
      }),
    });
  }

  /**
   * Creates a client in the database
   **/
  async function creatclient() {
    const response = await fetch("/api/PostClient", {
      method: "POST",
      headers: {
        "content-type": "application/json",
      },
    });
    // Get the Response Bodyo
    return await response.json();
  }
</script>
  <!-- The Shop Menu -->

<!-- The Search Bar -->
<div class="p-3 pt-0 mr-5">
  <label
    for="default-search"
    class="mb-2 text-sm font-medium text-gray-900 sr-only dark:text-white mr-5 pr-5"
  >
    Search
  </label>
  <div class="relative">
    <div
      class="absolute inset-y-0 start-0 flex items-center ps-3 pointer-events-none"
    >
      <svg
        class="w-4 h-4 text-gray-500 dark:text-gray-400"
        aria-hidden="true"
        xmlns="http://www.w3.org/2000/svg"
        fill="none"
        viewBox="0 0 20 20"
      >
        <path
          stroke="currentColor"
          stroke-linecap="round"
          stroke-linejoin="round"
          stroke-width="2"
          d="m19 19-4-4m0-7A7 7 0 1 1 1 8a7 7 0 0 1 14 0Z"
        />
      </svg>
    </div>
    <input
      bind:value={search}
      on:input={updateData}
      type="search"
      id="default-search"
      class="block w-full p-4 ps-10 text-sm text-gray-900 border border-gray-300 rounded-lg bg-gray-50 focus:ring-blue-500 focus:border-blue-500 dark:bg-gray-700 dark:border-gray-600 dark:placeholder-gray-400 dark:text-white dark:focus:ring-blue-500 dark:focus:border-blue-500"
      placeholder="Search Goods..."
      required
    />
  </div>

    <!-- The Goods -->
  <div class="pt-2">
    {#each data as record}
      <div
        class="flex flex-row items-center border border-gray-200 m-3 rounded-lg shadow md:flex-row md:max-w-xl hover:bg-gray-900 dark:border-gray-100 dark:bg-gray-100 dark:hover:bg-gray-100"
      >
        <img
          class="object-cover w-20 md:w-32 lg:w-48 m-w-20 max-w-100 md:rounded-3xl"
          src={record.photo.url}
          alt=""
        />

        <div class="flex flex-col">
          <h6
            class="mb-2 text-xl font-bold tracking-tight text-white dark:text-white align-top"
          >
            {record.name}
          </h6>
          <div class="flex flex-row">
            <div class="flex flex-col justify-between p-4 leading-normal">
              <p class="mb-3 font-normal text-gray-700 dark:text-gray-400">
                {record.calories} calories
              </p>
              <p class="mb-3 font-normal text-gray-700 dark:text-gray-400">
                type: {record.type}
              </p>
              <p />
              <p
                class="mb-3 font-normal text-gray-700 dark:text-gray-400 flex flex-row items-center"
              >
                {record.provider.name}
                <img
                  src={record.provider.photo.url}
                  alt=""
                  class="ml-5 w-10 h-10 rounded-full"
                />
              </p>
            </div>

            <div class="flex flex-col">
              <button
                id={record.id}
                on:click={addorder}
                class="addOrderBtn px-4 py-2 mt-2 text-sm font-medium text-white bg-blue-500 rounded-md hover:bg-blue-600 focus:outline-none focus:bg-blue-600"
              >
                Add
                {#if celebrate && record.id == selectedGood}
                  <Confetti duration={confettiDuration} />
                {/if}
              </button>
              <button
                title="info"
                class="px-4 py-2 mt-2 text-sm font-medium text-white bg-blue-500 rounded-md hover:bg-blue-600 focus:outline-none focus:bg-blue-600"
                >ℹ️
              </button>
            </div>
          </div>
        </div>
      </div>
    {/each}
  </div>
</div>
