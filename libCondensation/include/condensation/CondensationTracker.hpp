/*
 * CondensationTracker.hpp
 *
 *  Created on: 29.06.2012
 *      Author: poschmann
 */

#ifndef CONDENSATIONTRACKER_HPP_
#define CONDENSATIONTRACKER_HPP_

#include "condensation/Sample.hpp"
#include "opencv2/core/core.hpp"
#include "boost/optional.hpp"
#include <memory>
#include <vector>

namespace imageprocessing {
class VersionedImage;
}

namespace condensation {

class Sampler;
class MeasurementModel;
class StateExtractor;

/**
 * Tracker of a single target in image/video streams based on the Condensation algorithm (aka Particle Filter).
 */
class CondensationTracker {
public:

	/**
	 * Constructs a new condensation tracker.
	 *
	 * @param[in] sampler The sampler.
	 * @param[in] measurementModel The measurement model.
	 * @param[in] extractor The state extractor.
	 */
	CondensationTracker(std::shared_ptr<Sampler> sampler, std::shared_ptr<MeasurementModel> measurementModel,
			std::shared_ptr<StateExtractor> extractor);


	/**
	 * Processes the next image and returns the most probable target position.
	 *
	 * @param[in] image The next image.
	 * @return The bounding box around the most probable target position if found, none otherwise.
	 */
	boost::optional<cv::Rect> process(const cv::Mat& image);

	/**
	 * @return The estimated target state.
	 */
	std::shared_ptr<Sample> getState() {
		return state;
	}

	/**
	 * @return The current samples.
	 */
	inline const std::vector<std::shared_ptr<Sample>>& getSamples() const {
		return samples;
	}

	/**
	 * @return The sampler.
	 */
	inline std::shared_ptr<Sampler> getSampler() {
		return sampler;
	}

	/**
	 * @param[in] sampler The new sampler.
	 */
	inline void setSampler(std::shared_ptr<Sampler> sampler) {
		this->sampler = sampler;
	}

private:

	std::vector<std::shared_ptr<Sample>> samples;    ///< The current samples.
	std::vector<std::shared_ptr<Sample>> oldSamples; ///< The previous samples.
	std::shared_ptr<Sample> state;                   ///< The estimated target state.

	std::shared_ptr<imageprocessing::VersionedImage> image; ///< The image used for evaluation.
	std::shared_ptr<Sampler> sampler;                   ///< The sampler.
	std::shared_ptr<MeasurementModel> measurementModel; ///< The measurement model.
	std::shared_ptr<StateExtractor> extractor;          ///< The state extractor.
};

} /* namespace condensation */
#endif /* CONDENSATIONTRACKER_HPP_ */
