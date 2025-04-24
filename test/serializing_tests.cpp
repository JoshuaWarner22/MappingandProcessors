//
// Created by Davis Polito on 2/13/25.
//
#include <catch2/catch_test_macros.hpp>


#include "mapping.h"
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <iostream>
#include <catch2/catch_approx.hpp>
//bool areMappingsEqual(const leaf::tMapping& mapping1, const leaf::tMapping& mapping2)
//{
//  // Compare all non-pointer members
//  if (mapping1.uuid != mapping2.uuid) return false;
//  if (mapping1.destinationProcessorUniqueID != mapping2.destinationProcessorUniqueID) return false;
//  if (mapping1.index != mapping2.index) return false;
//  if (mapping1.paramID != mapping2.paramID) return false;
//  if (mapping1.numUsedSources != mapping2.numUsedSources) return false;
//
//  // Compare arrays
//  if (std::memcmp(mapping1.inUUIDS, mapping2.inUUIDS, MAX_NUM_SOURCES) != 0) return false;
//
//  // Compare atomic float arrays
//  for (size_t i = 0; i < MAX_NUM_SOURCES; ++i)
//  {
//      if (mapping1.bipolarOffset[i].load() != mapping2.bipolarOffset[i].load()) return false;
//      if (mapping1.scalingValues[i].load() != mapping2.scalingValues[i].load()) return false;
//  }
//
//  return true; // If all checks passed, they are equal
//}
// Helper function for atomic comparison
template <typename T>
bool atomicEqual(const std::atomic<T>& a, const std::atomic<T>& b) {
    return a.load() == b.load();
}

TEST_CASE("Test Mapping serialize deserialize", "[pssor]") {
    leaf::tMapping originalMapping{};

    // Initialize the original mapping
    originalMapping.uuid = 42;
    originalMapping.destinationProcessorUniqueID = 7;
    originalMapping.index = 0;
    originalMapping.paramID = 12;
    originalMapping.numUsedSources = 2;
    originalMapping.inUUIDS[0] = 15;
    originalMapping.inUUIDS[1] = 20;

    // Set atomic variables
    std::atomic<float> initialVal(1.0f);
    originalMapping.initialVal = &initialVal;
    originalMapping.bipolarOffset[0].store(0.25f);
    originalMapping.bipolarOffset[1].store(-0.25f);
    originalMapping.scalingValues[0].store(0.5f);
    originalMapping.scalingValues[1].store(1.5f);

    // Create a preset union for serialization
    leaf::tMappingPresetUnion serializedPreset{};

    // Serialize the original mapping into the preset union
    mapping_to_preset(&originalMapping, &serializedPreset);

    // Create another mapping structure to deserialize into
    leaf::tMapping deserializedMapping{};

    // Deserialize the preset union back into the mapping
    preset_to_mapping(serializedPreset, &deserializedMapping);

    // Compare the original and deserialized mappings field by field
    SECTION("Compare fields individually for granular feedback") {
        REQUIRE(originalMapping.uuid == deserializedMapping.uuid);
        REQUIRE(originalMapping.destinationProcessorUniqueID == deserializedMapping.destinationProcessorUniqueID);
        REQUIRE(originalMapping.index == deserializedMapping.index);
        REQUIRE(originalMapping.paramID == deserializedMapping.paramID);
        REQUIRE(originalMapping.numUsedSources == deserializedMapping.numUsedSources);

        // Verify inUUIDS array
        for (size_t i = 0; i < MAX_NUM_SOURCES; ++i) {
            INFO("Mismatch at inUUIDS[" << i << "]");
            REQUIRE(originalMapping.inUUIDS[i] == deserializedMapping.inUUIDS[i]);
        }

//        // Verify atomic float arrays
//        for (size_t i = 0; i < MAX_NUM_SOURCES; ++i) {
//            INFO("Mismatch at bipolarOffset[" << i << "] "<< originalMapping.bipolarOffset[i].load() << " " << deserializedMapping.bipolarOffset[i].load() << " ");
//            REQUIRE(atomicEqual(originalMapping.bipolarOffset[i], deserializedMapping.bipolarOffset[i]));
//
//            INFO("Mismatch at scalingValues[" << i << "] "  << originalMapping.scalingValues[i].load() << " " << deserializedMapping.scalingValues[i].load() << " ");
//            REQUIRE(atomicEqual(originalMapping.scalingValues[i], deserializedMapping.scalingValues[i]));
//        }
        // Verify atomic float arrays with Catch::WithinRel matcher
        for (size_t i = 0; i < MAX_NUM_SOURCES; ++i) {
            float a = originalMapping.bipolarOffset[i].load();
            float b = deserializedMapping.bipolarOffset[i].load();
            INFO("Checking bipolarOffset[" << i << "]");
            REQUIRE_THAT(a,
                         Catch::Matchers::WithinRel(b, 0.001f));

            INFO("Checking scalingValues[" << i << "]");
            REQUIRE_THAT(originalMapping.scalingValues[i].load(),
                         Catch::Matchers::WithinRel(deserializedMapping.scalingValues[i].load(), 0.001f));
        }


    }


}

TEST_CASE("Test Processor serialize deserialize", "[processor]") {
    leaf::tProcessor originalProcessor{};

    // Initialize the original processor
    originalProcessor.processorUniqueID = 42;
    originalProcessor.processorTypeID = 17;
    originalProcessor.proc_chain = 3;
    originalProcessor.index = 1;
    originalProcessor.numSetterFunctions = 4;

    // Setting up setter functions (mock values)
    for (size_t i = 0; i < MAX_NUM_PARAMS; ++i) {
        originalProcessor.setterFunctions[i] = reinterpret_cast<tSetter>(0xDEADBEEF + i);
    }

    // Initialize inParameters and outParameters
    std::atomic<float> inParams[MAX_NUM_PARAMS];
    std::atomic<float> outParams[MAX_NUM_PARAMS];
    for (size_t i = 0; i < MAX_NUM_PARAMS; ++i) {
        inParams[i].store(1.0f + i);  // Example: 1.0, 2.0, ...
        outParams[i].store(2.0f * i); // Example: 0.0, 2.0, 4.0, ...
    }
    originalProcessor.inParameters = inParams;
    originalProcessor.outParameters = outParams;

    // Create a processor preset union for serialization
    leaf::tProcessorPresetUnion serializedPreset{};

    // Serialize the original processor into the preset union
    processor_to_preset(&originalProcessor, &serializedPreset);

    // Create another processor structure to deserialize into
    leaf::tProcessor deserializedProcessor{};
    std::atomic<float> deserializedInParams[MAX_NUM_PARAMS];
    std::atomic<float> deserializedOutParams[MAX_NUM_PARAMS];

    // Initialize deserialized processor's pointers
    deserializedProcessor.inParameters = deserializedInParams;
    deserializedProcessor.outParameters = deserializedOutParams;

    // Deserialize the preset union back into the processor
    preset_to_processor(&serializedPreset, &deserializedProcessor);

    // Verify fields individually for granular feedback
    SECTION("Compare fields individually for precise relational feedback") {
        // Compare non-pointer fields
        REQUIRE(originalProcessor.processorUniqueID == deserializedProcessor.processorUniqueID);
        REQUIRE(originalProcessor.processorTypeID == deserializedProcessor.processorTypeID);
        REQUIRE(originalProcessor.proc_chain == deserializedProcessor.proc_chain);
        REQUIRE(originalProcessor.index == deserializedProcessor.index);
        REQUIRE(originalProcessor.numSetterFunctions == deserializedProcessor.numSetterFunctions);

        // Compare setterFunctions array
        for (size_t i = 0; i < MAX_NUM_PARAMS; ++i) {
            INFO("Mismatch at setterFunctions[" << i << "]");
            REQUIRE(reinterpret_cast<uintptr_t>(originalProcessor.setterFunctions[i]) ==
                    reinterpret_cast<uintptr_t>(deserializedProcessor.setterFunctions[i]));
        }

        // Compare inParameters
        for (size_t i = 0; i < MAX_NUM_PARAMS; ++i) {
            INFO("Checking inParameters[" << i << "]");
            REQUIRE_THAT(originalProcessor.inParameters[i].load(),
                         Catch::Matchers::WithinRel(deserializedProcessor.inParameters[i].load(), 0.001f));
        }

        // Compare outParameters
        for (size_t i = 0; i < MAX_NUM_PARAMS; ++i) {
            INFO("Checking outParameters[" << i << "]");
            REQUIRE_THAT(originalProcessor.outParameters[i].load(),
                         Catch::Matchers::WithinRel(deserializedProcessor.outParameters[i].load(), 0.001f));
        }
    }
}
TEST_CASE("Test size of MappingPreset and ProcessorPreset", "[sizes]") {
    SECTION("Print Sizes") {
        // Print the size of tMappingPreset
        std::cout << "Size of tMappingPreset: " << sizeof(leaf::tMappingPreset) << " bytes" << std::endl;

        // Print the size of tProcessorPreset
        std::cout << "Size of tProcessorPreset: " << sizeof(leaf::tProcessorPreset) << " bytes" << std::endl;
// Print sizes of each 7-bit struct
        std::cout << "Size of tMappingPreset7Bit: " << sizeof(leaf::tMappingPreset7Bit) << " bytes" << std::endl;
        std::cout << "Size of tProcessorPreset7Bit: " << sizeof(leaf::tProcessorPreset7Bit) << " bytes" << std::endl;

        // REQUIRE to ensure that sizes are non-zero for validation purposes
        REQUIRE(sizeof(leaf::tMappingPreset) > 0);
        REQUIRE(sizeof(leaf::tProcessorPreset) > 0);
    }
}
// Test case for splitting and unsplitting
TEST_CASE("tProcessorPreset split and unsplit operations are consistent", "[tProcessorPreset]") {
    // Create and initialize a tProcessorPreset instance
    leaf::tProcessorPreset originalPreset;
    originalPreset.processorTypeID = 84;
    originalPreset.processorUniqueID = 126;
    originalPreset.proc_chain = 21;
    originalPreset.index = 63;

    // Populate params with test float values
    for (int i = 0; i < MAX_NUM_PARAMS; ++i) {
        originalPreset.params[i] = static_cast<float>(i + 0.5f); // Example: 0.5, 1.5, 2.5, ...
    }

    // Initialize the 7-bit struct to store the split data
    leaf::tProcessorPreset7Bit preset7Bit;

    // Split the original preset
    leaf::splitProcessorPreset(&originalPreset, &preset7Bit);

    // Reconstruct the preset from the 7-bit data
    leaf::tProcessorPreset reconstructedPreset;
    leaf::unsplitProcessorPreset(&preset7Bit, &reconstructedPreset);

    // Verify that the reconstructed preset matches the original
    REQUIRE(reconstructedPreset.processorTypeID == originalPreset.processorTypeID);
    REQUIRE(reconstructedPreset.processorUniqueID == originalPreset.processorUniqueID);
    REQUIRE(reconstructedPreset.proc_chain == originalPreset.proc_chain);
    REQUIRE(reconstructedPreset.index == originalPreset.index);

    // Check each parameter
    for (int i = 0; i < MAX_NUM_PARAMS; ++i) {
        REQUIRE(reconstructedPreset.params[i] == Catch::Approx(originalPreset.params[i]));
    }
}


// Test splitting and unsplitting for tMappingPreset
TEST_CASE("tMappingPreset split and unsplit operations are consistent", "[tMappingPreset]") {
    // Create and initialize an example tMappingPreset
    leaf::tMappingPreset originalPreset;
    originalPreset.index = 84;
    originalPreset.uuid = 126;
    originalPreset.destinationUUID = 21;
    originalPreset.destParamID = 11;
    originalPreset.numUsedSources = 5;

    // Fill inUUIDs, bipolarOffset, and scalingValues with test data
    for (int i = 0; i < MAX_NUM_SOURCES; i++) {
        originalPreset.inUUIDs[i] = i + 1;
        originalPreset.bipolarOffset[i] = static_cast<float>(i) * 0.5f;
        originalPreset.scalingValues[i] = static_cast<float>(i) * 0.2f;
    }

    // Create the 7-bit struct
    leaf::tMappingPreset7Bit preset7Bit;

    // Split the preset into the 7-bit representation
    splitMappingPreset(&originalPreset, &preset7Bit);

    // Reconstruct the preset
    leaf::tMappingPreset reconstructedPreset;
    unsplitMappingPreset(&preset7Bit, &reconstructedPreset);

    // Verify that the reconstructed preset matches the original
    REQUIRE(reconstructedPreset.index == originalPreset.index);
    REQUIRE(reconstructedPreset.uuid == originalPreset.uuid);
    REQUIRE(reconstructedPreset.destinationUUID == originalPreset.destinationUUID);
    REQUIRE(reconstructedPreset.destParamID == originalPreset.destParamID);
    REQUIRE(reconstructedPreset.numUsedSources == originalPreset.numUsedSources);

    // Verify inUUIDs
    for (int i = 0; i < MAX_NUM_SOURCES; i++) {
        REQUIRE(reconstructedPreset.inUUIDs[i] == originalPreset.inUUIDs[i]);
    }

    // Verify bipolarOffset
    for (int i = 0; i < MAX_NUM_SOURCES; i++) {
        REQUIRE(reconstructedPreset.bipolarOffset[i] == Catch::Approx(originalPreset.bipolarOffset[i]));
    }

    // Verify scalingValues
    for (int i = 0; i < MAX_NUM_SOURCES; i++) {
        REQUIRE(reconstructedPreset.scalingValues[i] == Catch::Approx(originalPreset.scalingValues[i]));
    }
}

