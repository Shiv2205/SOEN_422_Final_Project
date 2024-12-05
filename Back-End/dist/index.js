"use strict";
var __awaiter = (this && this.__awaiter) || function (thisArg, _arguments, P, generator) {
    function adopt(value) { return value instanceof P ? value : new P(function (resolve) { resolve(value); }); }
    return new (P || (P = Promise))(function (resolve, reject) {
        function fulfilled(value) { try { step(generator.next(value)); } catch (e) { reject(e); } }
        function rejected(value) { try { step(generator["throw"](value)); } catch (e) { reject(e); } }
        function step(result) { result.done ? resolve(result.value) : adopt(result.value).then(fulfilled, rejected); }
        step((generator = generator.apply(thisArg, _arguments || [])).next());
    });
};
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
const express_1 = __importDefault(require("express"));
const cors_1 = __importDefault(require("cors"));
const dotenv_1 = __importDefault(require("dotenv"));
const cookie_parser_1 = __importDefault(require("cookie-parser"));
const HTTPCodes_1 = __importDefault(require("./Util/HTTPCodes"));
//Routes import
const auth_1 = __importDefault(require("./routes/auth"));
const attendance_1 = __importDefault(require("./routes/attendance"));
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
/**Routes */
app.use('/auth', auth_1.default);
app.use('/attendance', attendance_1.default);
app.get("/test", (req, res) => __awaiter(void 0, void 0, void 0, function* () {
    res.status(HTTPCodes_1.default.OK).send({ message: "Hello World" });
}));
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
    res.status(HTTPCodes_1.default.NOT_FOUND).send("Page not found!!!");
});
//Listen for requests
app.listen(PORT, () => {
    console.log(`Server listening on Port: ${PORT}`);
});
