#include "Section.h"

genmath::LongDouble printingsimulation::Section::XJerk = "0.0";
genmath::LongDouble printingsimulation::Section::YJerk = "0.0";
genmath::LongDouble printingsimulation::Section::XAcc = "0.0";
genmath::LongDouble printingsimulation::Section::YAcc = "0.0";
genmath::LongDouble printingsimulation::Section::MinResSpd = "0.0";
genmath::LongDouble printingsimulation::Section::MaxResSpd = "0.0";
genmath::LongDouble printingsimulation::Section::MaxResAcc = "0.0";
genmath::LongDouble printingsimulation::Section::MaxResJerk = "0.0";
genmath::LongDouble printingsimulation::Section::MaxResSpdTime = "0.0";
genmath::LongDouble printingsimulation::Section::MaxResSpdLen  = "0.0";
genmath::LongDouble printingsimulation::Section::Zero = "0.0";
genmath::LongDouble printingsimulation::Section::One = "1.0";
genmath::LongDouble printingsimulation::Section::Two = "2.0";
genmath::LongDouble printingsimulation::Section::Sixty = "60.0";// for feed rate conversion

void printingsimulation::Section::Init() {

	if(XJerk == Zero || YJerk == Zero || XAcc == Zero || YAcc == Zero || MinResSpd == Zero)
		throw std::exception("At least one global section parameter is not defined (Section).");

	// planar resultant jerk speed target
	Section::MaxResJerk =
		genmath::LongDouble::Sqrt((Section::XJerk * Section::XJerk) + (Section::YJerk * Section::YJerk));
	// planar resultant acceleration
	Section::MaxResAcc =
		genmath::LongDouble::Sqrt((Section::XAcc * Section::XAcc) + (Section::YAcc * Section::YAcc));
	// planar resultant speed
	// maximum resultant speed is configured via G0 or G1 feedrate
	Section::MaxResSpdTime = Section::MaxResSpd / Section::MaxResAcc;
	Section::MaxResSpdLen = (Section::One / Section::Two) * Section::MaxResAcc
		* Section::MaxResSpdTime * Section::MaxResSpdTime + Section::MaxResJerk * Section::MaxResSpdTime;
}

void printingsimulation::Section::UpdateGlobalResSpd(genmath::LongDouble new_res_spd) {

	Section::MaxResSpd = new_res_spd;
	
	// planar resultant speed
	// maximum resultant speed is configured via G0 or G1 feedrate
	Section::MaxResSpdTime = (Section::MaxResSpd) / Section::MaxResAcc;
	Section::MaxResSpdLen = (Section::One / Section::Two) * Section::MaxResAcc
		* Section::MaxResSpdTime * Section::MaxResSpdTime + Section::MaxResJerk * Section::MaxResSpdTime;
}

printingsimulation::Section::Section() {

	start_point_ = nullptr;
	end_point_ = nullptr;
	start_x_ = "0.0";
	start_y_ = "0.0";
	end_x_ = "0.0";
	end_y_ = "0.0";
	x_dir_ = "1.0";
	y_dir_ = "1.0";
	ratio_x_ = "1.0";
	ratio_y_ = "1.0";
	start_time_ = "0.0";
	end_time_ = "0.0";
	len_ = "0.0";
	fst_len_ = "0.0";
	snd_len_ = "0.0";
	avg_spd_ = "0.0";
	fst_avg_spd_ = "0.0";
	snd_avg_spd_ = "0.0";
	prev_sect_ = nullptr;
}

// creating the first section
printingsimulation::Section::Section(GCodeCommand<genmath::LongDouble>* cmd_param) {

	start_point_ = nullptr;
	end_x_ = cmd_param->Get("X");
	end_y_ = cmd_param->Get("Y");
	end_point_ = cmd_param;// will be modified at the next point, end point of current section
	start_x_ = end_x_;
	start_y_ = end_y_;
	x_dir_ = "1.0";
	y_dir_ = "1.0";
	ratio_x_ = "1.0";
	ratio_y_ = "1.0";
	start_time_ = "0.0";
	end_time_ = start_time_;// will be modified at the next point, end point of current section
	len_ = "0.0";
	fst_len_ = "0.0";
	snd_len_ = "0.0";
	avg_spd_ = "0.0";
	fst_avg_spd_ = "0.0";
	snd_avg_spd_ = "0.0";
	prev_sect_ = nullptr;
}

printingsimulation::Section::Section(const Section& orig) {

	start_point_ = orig.start_point_;
	end_point_ = orig.end_point_;
	start_x_ = orig.start_x_;
	start_y_ = orig.start_y_;
	end_x_ = orig.end_x_;
	end_y_ = orig.end_y_;
	x_dir_ = orig.x_dir_;
	y_dir_ = orig.y_dir_;
	ratio_x_ = orig.ratio_x_;
	ratio_y_ = orig.ratio_y_;
	start_time_ = orig.start_time_;
	end_time_ = orig.end_time_;
	len_ = orig.len_;
	avg_spd_ = orig.avg_spd_;
	fst_len_ = orig.fst_len_;
	snd_len_ = orig.snd_len_;
	fst_avg_spd_ = orig.fst_avg_spd_;
	snd_avg_spd_ = orig.snd_avg_spd_;
	prev_sect_ = nullptr;
}

printingsimulation::Section::~Section() {

}

printingsimulation::Section& printingsimulation::Section::operator=(const Section& orig) {

	start_point_ = orig.start_point_;
	end_point_ = orig.end_point_;
	start_x_ = orig.start_x_;
	start_y_ = orig.start_y_;
	end_x_ = orig.end_x_;
	end_y_ = orig.end_y_;
	x_dir_ = orig.x_dir_;
	y_dir_ = orig.y_dir_;
	ratio_x_ = orig.ratio_x_;
	ratio_y_ = orig.ratio_y_;
	start_time_ = orig.start_time_;
	end_time_ = orig.end_time_;
	len_ = orig.len_;
	avg_spd_ = orig.avg_spd_;
	fst_len_ = orig.fst_len_;
	snd_len_ = orig.snd_len_;
	fst_avg_spd_ = orig.fst_avg_spd_;
	snd_avg_spd_ = orig.snd_avg_spd_;
	prev_sect_ = nullptr;

	return *this;
}

std::pair<genmath::LongDouble, genmath::LongDouble> printingsimulation::Section::operator()(genmath::LongDouble t) {
	
	if (start_point_ == nullptr) [[unlikely]]
		throw std::exception("No start point defined, unfinished section (Section).");
	
	if (t < start_time_ || t > end_time_) [[unlikely]]
		throw std::exception(std::string("Requested time is out of range [start_time, end_time] (Section).").c_str());

	t = t - start_time_;
	
	return std::pair<genmath::LongDouble, genmath::LongDouble>({
		ratio_x_ * (x_dir_ * avg_spd_ * t) + start_x_,
		ratio_y_ * (y_dir_ * avg_spd_ * t) + start_y_ });
}

// This linearization is suboptimal respect to theoretical nonlinear time functions.
// Multiple average speed partitioning by time might increase precision of kinematic representation
// but requires more computation time.
void printingsimulation::Section::UpdateSection(GCodeCommand<genmath::LongDouble>* cmd_param, genmath::LongDouble& unit_time_step) {

	if (end_point_ == nullptr) [[unlikely]]
		throw std::exception("No start point defined. Ill initiated section (Section).");
	
	if (cmd_param == nullptr) [[unlikely]]
		throw std::exception("Null end point parameter. Ill initiated section (Section).");

	start_point_ = end_point_;
	end_point_ = cmd_param;
	start_x_ = end_x_;
	start_y_ = end_y_;
	end_x_ = end_point_->Get("X");
	end_y_ = end_point_->Get("Y");
	start_time_ = end_time_;

	genmath::LongDouble x_length = end_x_ - start_x_;
	genmath::LongDouble y_length = end_y_ - start_y_;
	
	x_dir_ = x_length < 0 ? "-1.0" : "1.0";
	y_dir_ = y_length < 0 ? "-1.0" : "1.0";

	if (x_length == Section::Zero) [[unlikely]] {
		
		ratio_x_ = "0.0";
		ratio_y_ = "1.0";
	}
	else if(y_length == Section::Zero){
		ratio_x_ = "1.0";
		ratio_y_ = "0.0";

	}
	else {
		ratio_x_ = genmath::LongDouble::Abs(y_length / x_length);
		ratio_y_ = genmath::LongDouble::Abs(x_length / y_length);
	}
	
	len_ = genmath::LongDouble::Sqrt((x_length * x_length) + (y_length * y_length));
	
	genmath::LongDouble subsect_ratio = len_ / Section::Two / Section::MaxResSpdLen;
	if(prev_sect_ == nullptr){
		// case: first section
	
		fst_len_ = subsect_ratio * Section::MaxResSpdLen;
		fst_avg_spd_ = Section::MaxResSpd / Section::Two;

		snd_len_ = subsect_ratio * Section::MaxResSpdLen;
		snd_avg_spd_ = Section::MaxResSpd / Section::Two;

		avg_spd_ = (fst_avg_spd_ + snd_avg_spd_) / Section::Two;
		end_time_ += len_ / avg_spd_;
		
		end_time_ = genmath::LongDouble::RawRound(end_time_, unit_time_step);
	}
	else{
		// case: second or nth section
	
		fst_len_ = subsect_ratio * Section::MaxResSpdLen;
		genmath::LongDouble post_ratio = fst_len_ / prev_sect_->snd_len_;
		prev_sect_->snd_avg_spd_ *= Section::Two * post_ratio;
		prev_sect_->avg_spd_ = (prev_sect_->fst_avg_spd_ + prev_sect_->snd_avg_spd_) / Section::Two;
		fst_avg_spd_ = prev_sect_->snd_avg_spd_;

		snd_len_ = subsect_ratio * Section::MaxResSpdLen;
		snd_avg_spd_ = Section::MaxResSpd / Section::Two;
		
		avg_spd_ = (fst_avg_spd_ + snd_avg_spd_) / Section::Two;
		end_time_ += len_ / avg_spd_;
		end_time_ = genmath::LongDouble::RawRound(end_time_, unit_time_step);
	}
}

void printingsimulation::Section::SetPrevSect(Section* prev_sect) {

	if (prev_sect == nullptr)
		throw std::exception("Previous section parameter is null (Section).");

	prev_sect_ = prev_sect;
}

void printingsimulation::Section::ConfigureSpeed(genmath::LongDouble& spd_mul) {

	if (spd_mul > Section::One)
		throw std::exception("Increased speed is not available (Section).");

	if (spd_mul > genmath::LongDouble("0.1")) {
	
		avg_spd_ *= spd_mul;
		fst_avg_spd_ *= spd_mul;
		snd_avg_spd_ *= spd_mul;
	
		if (avg_spd_ < Section::MinResSpd)
			throw std::exception("Reached minimum execution speed (Section).");

		end_time_ /= spd_mul;
		start_point_->Set("F", avg_spd_ * Section::Sixty);
	}
	else {
	
		throw std::exception("Speed multiplier factor is under 0.1 (Section).");
	}
}
