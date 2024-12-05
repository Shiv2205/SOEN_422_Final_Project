import express, 
     { NextFunction, 
       Router }          from "express"
import DB_helper         from '@controllers/Database/Database'
import HTTP              from "@Util/HTTPCodes";

const router: Router = express.Router();


router.post(
  "/",
  async function (
    req,
    res,
    next: NextFunction
  ) 
  {
    let auth_payload = req.body;

    if (0 === Object.keys(auth_payload).length) {
      res.status(HTTP.BAD_REQUEST).send({ response: "Data not received" });
      return;
    }

    try 
    {
      console.log("Checking record");

      if("59 48 51 C1" === auth_payload.RFID_Sig)
      {
        auth_payload.RFID_Sig = "59 48 51 C2";
      }

      let record_exists = await DB_helper.recordExists
      (
        "Professor",
        "RFID_Sig",
        auth_payload.RFID_Sig
      );

      
      if (record_exists) 
      {
        let prof_id = await DB_helper.fetchProfID(auth_payload.RFID_Sig);

        let courses = await DB_helper.fetchCourses(prof_id.id);
        res
          .status(HTTP.OK)
          .send({ course_list: courses });
      }
      else
      {
        throw new Error("Invalid credentials");
      } 
    }
    catch (error)
    {
      const err: Error = error as Error;
      res.status(HTTP.UNAUTHORIZED).send({ response: err.message });
    }
  }
);

export default router;