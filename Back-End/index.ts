import express, 
     { Request, 
       Response, 
       NextFunction }     from "express"
import cors               from "cors"
import dotenv             from "dotenv"
import cookieParser       from "cookie-parser"
import HTTP               from "@Util/HTTPCodes"
import createError        from "http-errors"

//Routes import


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

//Handle 404
app.use((req: Request, res: Response, next: NextFunction) => {
	next(createError(HTTP.NOT_FOUND, "Page not found!!!"));
});

//Listen for requests
app.listen(PORT, () => {
	console.log(`Server listening on Port: ${PORT}`);
});
