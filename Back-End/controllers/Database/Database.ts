import sqlite3    from 'sqlite3'
import { open }   from 'sqlite'
import { join }   from 'path'



async function getConnection()
{
  try 
  {
    const db = await open(
    {
      filename  : join(process.cwd(), '/data/database.sqlite'),
      driver    : sqlite3.Database,
    });

    console.log('Connected to SQLite');

    return db;
  } 
  catch (error) 
  {
    console.error('Database connection error:', error);
    throw error;
  }
};

async function recordExists(
  table_name: string,
  id_column_name: string,
  record_id: any
): Promise<boolean> 
{
  let DB = await getConnection();
  return new Promise<boolean>((resolve, reject) => 
  {
    DB.getDatabaseInstance().get
    (
      `SELECT COUNT(*) AS count FROM ${table_name} WHERE ${id_column_name} = ?;`,
      [record_id],
      function (err: Error | null, row: { count: number }) 
      {
        if( ! err)
        {
          resolve(row.count > 0);
        }
        else
        {
          reject(err);
        }
      }
    );
  }).finally(() => 
    {
      DB.close();
    });
}

async function isStudentRegistered(
  course_code: string,
  student_id: number
): Promise<boolean> 
{
  let DB = await getConnection();
  return new Promise<boolean>((resolve, reject) => 
  {
    DB.getDatabaseInstance().get
    (
      `SELECT COUNT(*) AS count 
       FROM  Course_registration 
       WHERE course_code = ? 
       AND   student_id  = ?;`,

      [course_code, student_id],
      function (err: Error | null, row: { count: number }) 
      {
        if( ! err)
        {
          resolve(row.count > 0);
        }
        else
        {
          reject(err);
        }
      }
    );
  }).finally(() => 
    {
      DB.close();
    });
}

async function isAttendanceLogged(
  course_code : string,
  student_id  : number,
  date        : string
): Promise<boolean> 
{
  let DB = await getConnection();
  return new Promise<boolean>((resolve, reject) => 
  {
    DB.getDatabaseInstance().get
    (
      `SELECT COUNT(*) AS count 
       FROM  Attendance_Log 
       WHERE course_code = ? 
       AND   student_id  = ?
       AND   recorded_on = ?;`,

      [course_code, student_id, date],
      function (err: Error | null, row: { count: number }) 
      {
        if( ! err)
        {
          resolve(row.count > 0);
        }
        else
        {
          reject(err);
        }
      }
    );
  }).finally(() => 
    {
      DB.close();
    });
}

async function logAttendance (
  course_code: string,
  student_id: number
)
{
  let DB            = await getConnection();
  let date_obj      = new Date();
  let date          = date_obj.toLocaleDateString();
  let time          = date_obj.toLocaleTimeString();
  let record_exists = await isAttendanceLogged(course_code, student_id, date); 
  return new Promise<{response: number}>((resolve, reject) =>
  {
    if( ! record_exists)
    {
      DB.getDatabaseInstance().run
      (
        `INSERT INTO Attendance_Log
        (student_id, course_code, recorded_on, recorded_at)
        VALUES 
        (?, ?, ?, ?);`,
        [student_id, course_code, date, time]
      );

      resolve({response: 201});
    }
    else
    {
      resolve({response: 500});
    }
  }).finally(() => 
    {
      DB.close();
    });
}

async function fetchProfID (professor_rfid: string)
{
  let DB = await getConnection();
  return new Promise<{id: number}>(async (resolve, reject) =>
  {
    DB.getDatabaseInstance().get
    (
      `SELECT id FROM Professor WHERE RFID_Sig = ?;`,
      [professor_rfid],
      function (err: Error | null, row: {id: number}) 
      {
        if( ! err)
        {
          resolve(row);
        }
        else
        {
          reject(err);
        }
      }
    );
  }).finally(() => 
    {
      DB.close();
    });
}

async function fetchCourses (professor_id: number)
{
  let DB = await getConnection();
  return new Promise<{code: string}[]>(async (resolve, reject) =>
  {
    DB.getDatabaseInstance().all
    (
      `SELECT code FROM Course WHERE professor_id = ?;`,
      [professor_id],
      function (err: Error | null, row: {code: string}[]) 
      {
        if( ! err)
        {
          resolve(row);
        }
        else
        {
          reject(err);
        }
      }
    );
  }).finally(() => 
    {
      DB.close();
    });
}

async function fetchStudID (RFID_Sig: string)
{
  let DB = await getConnection();
  return new Promise<{id: number}>(async (resolve, reject) =>
  {
    DB.getDatabaseInstance().get
    (
      `SELECT id FROM Student WHERE RFID_Sig = ?;`,
      [RFID_Sig],
      function (err: Error | null, row: {id: number}) 
      {
        if( ! err)
        {
          resolve(row);
        }
        else
        {
          reject(err);
        }
      }    
    );
  }).finally(() => 
    {
      DB.close();
    });
}

//Default export
const DBController = 
{
  //Methods
  getConnection,
  recordExists,

  isStudentRegistered,
  isAttendanceLogged,

  logAttendance,

  fetchProfID,
  fetchStudID,
  fetchCourses
};

export default DBController;
