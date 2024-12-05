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
    let attendance_payload = req.body;

    if (0 === Object.keys(attendance_payload).length) {
      res.status(HTTP.BAD_REQUEST).send({ response: "Data not received" });
      return;
    }

    try 
    {
      let student_id    = (await DB_helper.fetchStudID(attendance_payload.RFID_Sig)).id;

      let isRegistered  = await DB_helper.isStudentRegistered
      (
        attendance_payload.course_code,
        student_id
      );

      if( ! isRegistered)
        {
          throw new Error("Student not registered for the course.");
      }

      let response = await DB_helper.logAttendance
      (
        attendance_payload.course_code,
        student_id
      );

      res
      .status(HTTP.CREATED)
      .send(response);
    }
    catch (error)
    {
      const err: Error = error as Error;
      res.status(HTTP.BAD_REQUEST).send({ response: err.message });
    }
  }
);

export default router;