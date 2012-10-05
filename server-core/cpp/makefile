concept_proof: concept_proof.cpp
	./to_ucn.sh concept_proof.cpp
	./to_ucn.sh sites.h
	cp -f trains.h /tmp/trains.h
	cp -f site_names.h /tmp/site_names.h
	g++ -g -fextended-identifiers -lboost_unit_test_framework -o concept_proof /tmp/concept_proof.cpp