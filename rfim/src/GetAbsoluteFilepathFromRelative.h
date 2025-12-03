#ifndef INCLUDE_RFIM_GET_ABSOLUTE_FILEPATH_FROM_RELATIVE
#define INCLUDE_RFIM_GET_ABSOLUTE_FILEPATH_FROM_RELATIVE

#include<string>
#include<stdexcept>

namespace {
	std::string GetAbsoluteFilepathFromRelative(std::string relative_filepath, std::string reference_filepath)
	{
		bool reference_is_folderpath = reference_filepath.rfind(".", reference_filepath.length()) == std::string::npos;
		if (reference_is_folderpath)
		{
			if (reference_filepath.compare(reference_filepath.length() - 1, 1, "\\") == 1 &&
				reference_filepath.compare(reference_filepath.length() - 1, 1, "/") == 1)
			{
				reference_filepath += "/";
			}
			return reference_filepath + relative_filepath;
		}

		size_t last_back_slash_idx = reference_filepath.rfind('\\', reference_filepath.length());
		if (last_back_slash_idx == std::string::npos) { last_back_slash_idx = 0; }
		size_t last_forwards_lash_idx = reference_filepath.rfind('/', reference_filepath.length());
		if (last_forwards_lash_idx == std::string::npos) { last_forwards_lash_idx = 0; }

		const size_t last_slash_idx = std::max(last_back_slash_idx, last_forwards_lash_idx);

		if (std::string::npos == 0)
		{
			std::string error_string = 
				std::string("No '\\' or '/' characters were detected as folder seperators in absolute filepath when trying to get the relative filepath in rfim::GetAbsoluteFilepathFromRelative");
			throw std::logic_error(error_string);
		}

		std::string reference_folderpath = reference_filepath.substr(0, last_slash_idx + 1);
		return reference_folderpath + relative_filepath;
	}

} // namespace: rfim
#endif