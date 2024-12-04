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
const sqlite3_1 = __importDefault(require("sqlite3"));
const sqlite_1 = require("sqlite");
const path_1 = require("path");
function getConnection() {
    return __awaiter(this, void 0, void 0, function* () {
        try {
            const db = yield (0, sqlite_1.open)({
                filename: (0, path_1.join)(process.cwd(), '/data/database.sqlite'),
                driver: sqlite3_1.default.Database,
            });
            console.log('Connected to SQLite');
            return db;
        }
        catch (error) {
            console.error('Database connection error:', error);
            throw error;
        }
    });
}
;
function recordExists(table_name, id_column_name, record_id) {
    return __awaiter(this, void 0, void 0, function* () {
        let DB = yield getConnection();
        return new Promise((resolve, reject) => {
            DB.getDatabaseInstance().get(`SELECT COUNT(*) AS count FROM ${table_name} WHERE ${id_column_name} = ?;`, [record_id], function (err, row) {
                if (!err) {
                    resolve(row.count > 0);
                }
                else {
                    reject(err);
                }
            });
        }).finally(() => {
            DB.close();
        });
    });
}
function fetchProfID(professor_rfid) {
    return __awaiter(this, void 0, void 0, function* () {
        let DB = yield getConnection();
        return new Promise((resolve, reject) => __awaiter(this, void 0, void 0, function* () {
            DB.getDatabaseInstance().get(`SELECT id FROM Professor WHERE RFID_Sig = ?;`, [professor_rfid], function (err, row) {
                //DB.close();
                if (!err) {
                    resolve(row);
                }
                else {
                    reject(err);
                }
            });
        })).finally(() => {
            DB.close();
        });
    });
}
function fetchCourses(professor_id) {
    return __awaiter(this, void 0, void 0, function* () {
        let DB = yield getConnection();
        return new Promise((resolve, reject) => __awaiter(this, void 0, void 0, function* () {
            DB.getDatabaseInstance().all(`SELECT code FROM Course WHERE professor_id = ?;`, [professor_id], function (err, row) {
                //DB.close();
                if (!err) {
                    console.log("Courses Row: ", row);
                    resolve(row);
                }
                else {
                    reject(err);
                }
            });
        })).finally(() => {
            DB.close();
        });
    });
}
//Default export
const DBController = {
    //Methods
    getConnection,
    recordExists,
    fetchProfID,
    fetchCourses
};
exports.default = DBController;
