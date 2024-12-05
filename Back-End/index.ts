import express, { Request, 
				          Response,
                  NextFunction }  from 'express'
import cors                       from "cors"
import dotenv                     from "dotenv"
import cookieParser               from "cookie-parser"
import HTTP                       from "@Util/HTTPCodes"
//Routes import
import authRouter                 from "@routes/auth"
import attendanceRouter          from "@routes/attendance"


//Dev Consts
const HOPPSCOTCH = "chrome-extension://amknoiejhlmhancpahfcfcfhllgkpbld";

//Express Init
dotenv.config(); //Load environment variables from .env file
const app     = express();
const PORT    = process.env.PORT   || 8000;
const CLIENT  = process.env.CLIENT || "http://localhost:3000";


//Express middleware config
app.use(express.urlencoded({ extended: false }));
app.use(express.json());
app.use(cookieParser());
app.use(cors({ origin: [HOPPSCOTCH, CLIENT, "*"] }));

/**Routes */
app.use('/auth', authRouter);
app.use('/attendance', attendanceRouter);

app.get("/test", async(req, res) => 
{
  res.status(HTTP.OK).send({ message: "Hello World"});
})

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
app.use((req: Request, res: Response, next: NextFunction) => {
	res.status(HTTP.NOT_FOUND).send("Page not found!!!");
});

//Listen for requests
app.listen(PORT, () => {
	console.log(`Server listening on Port: ${PORT}`);
});
