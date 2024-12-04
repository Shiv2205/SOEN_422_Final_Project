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

//Default export
const DBController = 
{
  //Methods
  getConnection,
  recordExists,

  fetchProfID,
  fetchCourses
};

export default DBController;
