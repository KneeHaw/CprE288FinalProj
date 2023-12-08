<script>
  // author: Conner Ohnesorge (2023) | conneroh.com
</script>
<div
    id="static-modal"
    data-modal-backdrop="static"
    tabindex="-1"
    aria-hidden="true"
    class="hidden overflow-y-auto overflow-x-hidden fixed top-0 right-0 left-0 z-50 justify-center items-center w-full md:inset-0 h-[calc(100%-1rem)] max-h-full"
>
    <div class="relative p-4 w-full max-w-2xl max-h-full">
        <div class="flex flex-col w-full min-h-screen p-8">
            <header class="flex items-center justify-between mb-8">
                    <svg
                        xmlns="http://www.w3.org/2000/svg"
                        width="24"
                        height="24"
                        viewBox="0 0 24 24"
                        fill="none"
                        stroke="currentColor"
                        stroke-width="2"
                        stroke-linecap="round"
                        stroke-linejoin="round"
                        class="h-6 w-6 mr-2"
                        ><path d="m8 3 4 8 5-5 5 15H2L8 3z"></path></svg
                    ><span class="text-2xl font-semibold">Roomba Dash</span
                ><h1 class="text-2xl font-bold">Rate Your Driver</h1>
            </header>
            <div
                class="rounded-lg border bg-card text-card-foreground shadow-sm"
                data-v0-t="card"
            >
                <div class="flex flex-col space-y-1.5 p-6">
                    <div class="flex items-center gap-4">
                        <span
                            class="relative flex shrink-0 overflow-hidden rounded-full w-16 h-16 border"
                            ><img
                                class="aspect-square h-full w-full"
                                alt="Driver's Name"
                                src="/placeholder-user.jpg"
                            /><span
                                class="flex h-full w-full items-center justify-center rounded-full bg-muted"
                                >DN</span
                            ></span
                        >
                        <h2 class="text-xl font-bold">Driver's Name</h2>
                    </div>
                </div>
                <div class="p-6">
                    <div class="flex items-center gap-4 mb-4">
                        <svg
                            xmlns="http://www.w3.org/2000/svg"
                            width="24"
                            height="24"
                            viewBox="0 0 24 24"
                            fill="none"
                            stroke="currentColor"
                            stroke-width="2"
                            stroke-linecap="round"
                            stroke-linejoin="round"
                            class="w-8 h-8 fill-muted stroke-muted-foreground"
                            ><polygon
                                points="12 2 15.09 8.26 22 9.27 17 14.14 18.18 21.02 12 17.77 5.82 21.02 7 14.14 2 9.27 8.91 8.26 12 2"
                            ></polygon></svg
                        ><svg
                            xmlns="http://www.w3.org/2000/svg"
                            width="24"
                            height="24"
                            viewBox="0 0 24 24"
                            fill="none"
                            stroke="currentColor"
                            stroke-width="2"
                            stroke-linecap="round"
                            stroke-linejoin="round"
                            class="w-8 h-8 fill-muted stroke-muted-foreground"
                            ><polygon
                                points="12 2 15.09 8.26 22 9.27 17 14.14 18.18 21.02 12 17.77 5.82 21.02 7 14.14 2 9.27 8.91 8.26 12 2"
                            ></polygon></svg
                        ><svg
                            xmlns="http://www.w3.org/2000/svg"
                            width="24"
                            height="24"
                            viewBox="0 0 24 24"
                            fill="none"
                            stroke="currentColor"
                            stroke-width="2"
                            stroke-linecap="round"
                            stroke-linejoin="round"
                            class="w-8 h-8 fill-muted stroke-muted-foreground"
                            ><polygon
                                points="12 2 15.09 8.26 22 9.27 17 14.14 18.18 21.02 12 17.77 5.82 21.02 7 14.14 2 9.27 8.91 8.26 12 2"
                            ></polygon></svg
                        ><svg
                            xmlns="http://www.w3.org/2000/svg"
                            width="24"
                            height="24"
                            viewBox="0 0 24 24"
                            fill="none"
                            stroke="currentColor"
                            stroke-width="2"
                            stroke-linecap="round"
                            stroke-linejoin="round"
                            class="w-8 h-8 fill-muted stroke-muted-foreground"
                            ><polygon
                                points="12 2 15.09 8.26 22 9.27 17 14.14 18.18 21.02 12 17.77 5.82 21.02 7 14.14 2 9.27 8.91 8.26 12 2"
                            ></polygon></svg
                        ><svg
                            xmlns="http://www.w3.org/2000/svg"
                            width="24"
                            height="24"
                            viewBox="0 0 24 24"
                            fill="none"
                            stroke="currentColor"
                            stroke-width="2"
                            stroke-linecap="round"
                            stroke-linejoin="round"
                            class="w-8 h-8 fill-muted stroke-muted-foreground"
                            ><polygon
                                points="12 2 15.09 8.26 22 9.27 17 14.14 18.18 21.02 12 17.77 5.82 21.02 7 14.14 2 9.27 8.91 8.26 12 2"
                            ></polygon></svg
                        >
                    </div>
                    <div class="mb-4">
                        <label
                            class="font-medium peer-disabled:cursor-not-allowed peer-disabled:opacity-70 text-base"
                            for="feedback"
                        >
                            Additional Feedback
                        </label><input
                            class="flex h-10 bg-background text-sm ring-offset-background file:border-0 file:bg-transparent file:text-sm file:font-medium placeholder:text-muted-foreground focus-visible:outline-none focus-visible:ring-2 focus-visible:ring-ring focus-visible:ring-offset-2 disabled:cursor-not-allowed disabled:opacity-50 w-full p-2 border border-zinc-200 rounded-md dark:border-zinc-800"
                            id="feedback"
                            type="text"
                        />
                    </div>
                    <button
                        class="inline-flex items-center justify-center text-sm font-medium ring-offset-background transition-colors focus-visible:outline-none focus-visible:ring-2 focus-visible:ring-ring focus-visible:ring-offset-2 disabled:pointer-events-none disabled:opacity-50 hover:bg-primary/90 h-10 px-4 w-full py-2 bg-blue-500 text-white rounded-md"
                        type="submit">Submit</button
                    >
                </div>
            </div>
        </div>
    </div>
</div>
