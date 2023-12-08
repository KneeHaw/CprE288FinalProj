// author: Conner Ohnesorge (2023) | conneroh.com

// Import the client from the xata package
import { buildClient } from "@xata.io/client";

// Import the types from the xata package
import type {
  BaseClientOptions,
  SchemaInference,
  XataRecord,
} from "@xata.io/client";

// The tables in the database defined code-first
const tables = [
  {
    name: "roomba",
    columns: [
      { name: "latitude", type: "float", notNull: true, defaultValue: "0" },
      { name: "longitude", type: "float", notNull: true, defaultValue: "0" },
      { name: "degrees", type: "float" },
    ],
  },
  {
    name: "carts",
    columns: [
      {
        name: "in-progress",
        type: "bool",
        notNull: true,
        defaultValue: "false",
      },
      { name: "good", type: "link", link: { table: "goods" } },
      { name: "client", type: "link", link: { table: "clients" } },
    ],
  },
  {
    name: "plotter",
    columns: [
      { name: "latitude", type: "float", notNull: true, defaultValue: "0" },
      { name: "longitude", type: "float", notNull: true, defaultValue: "0" },
    ],
  },
  {
    name: "goods",
    columns: [
      { name: "name", type: "string", unique: true },
      { name: "photo", type: "file" },
      { name: "calories", type: "int", notNull: true, defaultValue: "0" },
      {
        name: "type",
        type: "string",
        notNull: true,
        defaultValue: "undefined",
      },
      { name: "provider", type: "link", link: { table: "businesses" } },
      { name: "country_origin", type: "string" },
      { name: "subtype", type: "string" },
      { name: "price", type: "float" },
    ],
    revLinks: [{ column: "good", table: "carts" }],
  },
  {
    name: "businesses",
    columns: [
      { name: "name", type: "string", unique: true },
      { name: "latitude", type: "float", notNull: true, defaultValue: "0" },
      { name: "longitude", type: "float", notNull: true, defaultValue: "0" },
      { name: "photo", type: "file" },
    ],
    revLinks: [{ column: "provider", table: "goods" }],
  },
  {
    name: "houses",
    columns: [
      { name: "latitude", type: "float", notNull: true, defaultValue: "0" },
      { name: "longitude", type: "float", notNull: true, defaultValue: "0" },
      {
        name: "name",
        type: "string",
        notNull: true,
        defaultValue: "undefined",
      },
      { name: "character", type: "string" },
    ],
    revLinks: [
      { column: "house", table: "clients" },
      { column: "house", table: "orders" },
    ],
  },
  {
    name: "obstacles",
    columns: [
      { name: "latitude", type: "float", notNull: true, defaultValue: "0" },
      { name: "longitude", type: "float", notNull: true, defaultValue: "0" },
    ],
  },
  {
    name: "plan",
    columns: [
      { name: "latitude", type: "float", notNull: true, defaultValue: "0" },
      { name: "longitude", type: "float", notNull: true, defaultValue: "0" },
    ],
  },
  {
    name: "clients",
    columns: [
      { name: "ipaddress", type: "string", unique: true },
      { name: "house", type: "link", link: { table: "houses" } },
      { name: "name", type: "string" },
    ],
    revLinks: [
      { column: "client", table: "carts" },
      { column: "client", table: "orders" },
    ],
  },
  { name: "country", columns: [{ name: "name", type: "string" }] },
  {
    name: "orders",
    columns: [
      { name: "goods", type: "multiple" },
      { name: "house", type: "link", link: { table: "houses" } },
      { name: "client", type: "link", link: { table: "clients" } },
      { name: "total_price", type: "float" },
      {
        name: "status",
        type: "string",
        notNull: true,
        defaultValue: "Preparing",
      },
    ],
  },
  {
    name: "stop_signs",
    columns: [
      { name: "latitude", type: "float", notNull: true, defaultValue: "0" },
      { name: "longitude", type: "float", notNull: true, defaultValue: "0" },
    ],
  },
] as const;

// The type of the tables in the database
export type SchemaTables = typeof tables;
export type InferredTypes = SchemaInference<SchemaTables>;

// The type of the record for the roomba table
export type Roomba = InferredTypes["roomba"];
export type RoombaRecord = Roomba & XataRecord;

// The type of the record for the carts table
export type Carts = InferredTypes["carts"];
export type CartsRecord = Carts & XataRecord;

// The type of the record for the plotter table
export type Plotter = InferredTypes["plotter"];
export type PlotterRecord = Plotter & XataRecord;

// The type of the record for the goods table
export type Goods = InferredTypes["goods"];
export type GoodsRecord = Goods & XataRecord;

// The type of the record for the businesses table
export type Businesses = InferredTypes["businesses"];
export type BusinessesRecord = Businesses & XataRecord;

// The type of the record for the houses table
export type Houses = InferredTypes["houses"];
export type HousesRecord = Houses & XataRecord;

// The type of the record for the obstacles table
export type Obstacles = InferredTypes["obstacles"];
export type ObstaclesRecord = Obstacles & XataRecord;

// The type of the record for the plan table
export type Plan = InferredTypes["plan"];
export type PlanRecord = Plan & XataRecord;

// The type of the record for the clients table
export type Clients = InferredTypes["clients"];
export type ClientsRecord = Clients & XataRecord;

// The type of the record for the country table
export type Country = InferredTypes["country"];
export type CountryRecord = Country & XataRecord;

// The type of the record for the orders table
export type Orders = InferredTypes["orders"];
export type OrdersRecord = Orders & XataRecord;

// The type of the record for the stop_signs table
export type StopSigns = InferredTypes["stop_signs"];
export type StopSignsRecord = StopSigns & XataRecord;

// The schema of the database
export type DatabaseSchema = {
  roomba: RoombaRecord;
  carts: CartsRecord;
  plotter: PlotterRecord;
  goods: GoodsRecord;
  businesses: BusinessesRecord;
  houses: HousesRecord;
  obstacles: ObstaclesRecord;
  plan: PlanRecord;
  clients: ClientsRecord;
  country: CountryRecord;
  orders: OrdersRecord;
  stop_signs: StopSignsRecord;
};

// The client for the database
const DatabaseClient = buildClient();

// The default options for the client
const defaultOptions = {
  databaseURL: import.meta.env.XATA_DATABASE_URL,
  apiKey: import.meta.env.XATA_API_KEY,
};

export class XataClient extends DatabaseClient<DatabaseSchema> {
  constructor(options?: BaseClientOptions) {
    super({ ...defaultOptions, ...options }, tables);
  }
}

// The instance of the client
let instance: XataClient | undefined = undefined;

// Get the client for the database. This is used to interact with the database.
export const getXataClient = () => {
  if (instance) return instance;

  instance = new XataClient();
  return instance;
};
