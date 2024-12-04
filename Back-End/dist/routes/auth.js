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
const Database_1 = __importDefault(require("../controllers/Database/Database"));
const HTTPCodes_1 = __importDefault(require("../Util/HTTPCodes"));
const router = express_1.default.Router();
router.post("/", function (req, res, next) {
    return __awaiter(this, void 0, void 0, function* () {
        let auth_payload = req.body;
        if (Object.keys(auth_payload).length === 0) {
            res.status(HTTPCodes_1.default.BAD_REQUEST).send({ response: "Data not received" });
            return;
        }
        try {
            console.log("Checking record");
            let record_exists = yield Database_1.default.recordExists("Professor", "RFID_Sig", auth_payload.RFID_Sig);
            if (record_exists) {
                let prof_id = yield Database_1.default.fetchProfID(auth_payload.RFID_Sig);
                let courses = yield Database_1.default.fetchCourses(prof_id.id);
                res
                    .status(HTTPCodes_1.default.OK)
                    .send({ course_list: courses });
            }
            else {
                throw new Error("Invalid credentials");
            }
        }
        catch (error) {
            const err = error;
            res.status(HTTPCodes_1.default.UNAUTHORIZED).send({ response: err.message });
        }
    });
});
exports.default = router;
