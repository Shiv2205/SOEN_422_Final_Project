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

    if (Object.keys(auth_payload).length === 0) {
      res.status(HTTP.BAD_REQUEST).send({ response: "Data not received" });
      return;
    }

    try 
    {
      console.log("Checking record");
      
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