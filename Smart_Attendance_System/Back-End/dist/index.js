"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
const express_1 = __importDefault(require("express"));
const cors_1 = __importDefault(require("cors"));
const dotenv_1 = __importDefault(require("dotenv"));
const cookie_parser_1 = __importDefault(require("cookie-parser"));
//Routes import
//Dev Consts
const HOPPSCOTCH = "chrome-extension://amknoiejhlmhancpahfcfcfhllgkpbld";
//Express Init
dotenv_1.default.config(); //Load environment variables from .env file
const app = (0, express_1.default)();
const PORT = process.env.PORT || 8000;
const CLIENT = process.env.CLIENT || "http://localhost:3000";
//Express middleware config
app.use(express_1.default.urlencoded({ extended: false }));
app.use(express_1.default.json());
app.use((0, cookie_parser_1.default)());
app.use((0, cors_1.default)({ origin: [HOPPSCOTCH, CLIENT, "*"] }));
/**
 * DB test route
 * TO BE REMOVED
 */
// app.get("/test-db", async (req, res) => {
// 	try {
// 		const pool = await Database.getConnection();
// 		if (pool) {
// 			const result = await pool.request().query("SELECT 1 AS number");
// 			res.status(HTTP.OK).send({
// 				message: "Database connected successfully!",
// 				result: result.recordset,
// 			});
// 		} else {
// 			throw new Error("Connection error in test-db");
// 		}
// 	} catch (error) {
// 		res
// 			.status(HTTP.SERVER_ERR)
// 			.send({ message: "Database connection failed", error });
// 	}
// });
//Handle 404
app.use((req, res, next) => {
    // next(createError(HTTP.NOT_FOUND, "Page not found!!!"));
});
//Listen for requests
app.listen(PORT, () => {
    console.log(`Server listening on Port: ${PORT}`);
});
