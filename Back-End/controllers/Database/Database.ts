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
      driver    :   sqlite3.Database,
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



//Default export
const DBController = 
{
  //Methods
  getConnection
};

export default DBController;
