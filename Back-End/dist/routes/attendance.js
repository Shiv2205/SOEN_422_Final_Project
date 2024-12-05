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
        let attendance_payload = req.body;
        if (0 === Object.keys(attendance_payload).length) {
            res.status(HTTPCodes_1.default.BAD_REQUEST).send({ response: "Data not received" });
            return;
        }
        try {
            let student_id = (yield Database_1.default.fetchStudID(attendance_payload.RFID_Sig)).id;
            let isRegistered = yield Database_1.default.isStudentRegistered(attendance_payload.course_code, student_id);
            if (!isRegistered) {
                throw new Error("Student not registered for the course.");
            }
            let response = yield Database_1.default.logAttendance(attendance_payload.course_code, student_id);
            res
                .status(HTTPCodes_1.default.CREATED)
                .send(response);
        }
        catch (error) {
            const err = error;
            res.status(HTTPCodes_1.default.BAD_REQUEST).send({ response: err.message });
        }
    });
});
exports.default = router;
